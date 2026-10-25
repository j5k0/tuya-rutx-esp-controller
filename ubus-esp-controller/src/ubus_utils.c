#include "ubus_utils.h"
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>

enum {
    RETURN_RC,
    RETURN_MSG,
    RETURN_DATA,
    __RETURN_MAX
};

const struct blobmsg_policy return_policy[__RETURN_MAX] = {
    [RETURN_RC] = { .name = "rc", .type = BLOBMSG_TYPE_INT32 },
    [RETURN_MSG] = { .name = "msg", .type = BLOBMSG_TYPE_STRING },
    [RETURN_DATA] = { .name = "data", .type = BLOBMSG_TYPE_TABLE }
};

int check_args(struct ubus_context *ctx, struct ubus_request_data *req, const struct blobmsg_policy *policy, int end, struct blob_attr *args[]){
    for(int i=0; i<end; i++){
        if(args[i] == NULL){
            char error_buf[MAX_BUFFER_SIZE];
            snprintf(error_buf, MAX_BUFFER_SIZE, "Invalid argument: %s", policy[i].name);
            return_error_msg(ctx, req, UBUS_STATUS_INVALID_ARGUMENT, error_buf);
            return UBUS_STATUS_INVALID_ARGUMENT;
        }
    }
    return 0;
}

static void append_data_blob(struct blob_buf *dest, struct blob_buf *src){
    void *table = blobmsg_open_table(dest, "data");
    struct blob_attr *cur;
    size_t rem;
    blobmsg_for_each_attr(cur, src->head, rem)
        blobmsg_add_blob(dest, cur);
    blobmsg_close_table(dest, table);
}

void return_success_msg(struct ubus_context *ctx, struct ubus_request_data *req, struct blob_buf *data){
    struct blob_buf resp = {};
    blob_buf_init(&resp, 0);
    blobmsg_add_u32(&resp, "response_code", 0);
    blobmsg_add_string(&resp, "message", "Success");
    if(data != NULL)
        append_data_blob(&resp, data);
    ubus_send_reply(ctx, req, resp.head);
    blob_buf_free(&resp);
}

void return_error_msg(struct ubus_context *ctx, struct ubus_request_data *req, int error_code, char *string){
    struct blob_buf resp = {};
    blob_buf_init(&resp, 0);
    blobmsg_add_u32(&resp, "response_code", error_code);
    blobmsg_add_string(&resp, "message", string);
    ubus_send_reply(ctx, req, resp.head);
    blob_buf_free(&resp);
}

void build_devices_blob(struct blob_buf *b, struct port ports[], int n){
    void *table;
    char buffer[8];
    for(int i=0; i<n; i++){
        table = blobmsg_open_table(b, ports[i].serial_id);
        blobmsg_add_string(b, "port", ports[i].port);
        snprintf(buffer, 8, "0x%04X", ports[i].vendor_id);
        blobmsg_add_string(b, "vendor id", buffer);
        snprintf(buffer, 8, "0x%04X", ports[i].product_id);
        blobmsg_add_string(b, "product id", buffer);
        blobmsg_close_table(b, table);
    }
}

int reformat_port_info_blob(struct ubus_context *ctx, struct ubus_request_data *req, struct blob_buf *dest, char *buffer){
    struct blob_attr *tb[__RETURN_MAX];
    struct blob_buf tmp = {};
    blob_buf_init(&tmp, 0);
    blobmsg_add_json_from_string(&tmp, buffer);
    blobmsg_parse(return_policy, __RETURN_MAX, tb, blob_data(tmp.head), blob_len(tmp.head));
    if(tb[RETURN_DATA] == NULL){
        return_error_msg(ctx, req, UBUS_STATUS_UNKNOWN_ERROR, "Failed to format response");
        blob_buf_free(&tmp);
        return UBUS_STATUS_UNKNOWN_ERROR;
    }
    blob_buf_free(&tmp);

    struct blob_attr *cur;
    size_t rem;
    blobmsg_for_each_attr(cur, tb[RETURN_DATA], rem) {
        const char* name = blobmsg_name ( cur );
        if ( strcmp ( "temperature", name ) == 0 )
            blobmsg_add_blob(dest, cur);
        else if ( strcmp ( "humidity", name ) == 0 )
            blobmsg_add_blob(dest, cur);
    }

    return UBUS_STATUS_OK;
}