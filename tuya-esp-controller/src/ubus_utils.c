#include "ubus_utils.h"
#include "globals.h"
#include <libubox/blobmsg_json.h>
#include <syslog.h>

void return_error_msg(int error_code, char *string){
    struct blob_buf buf = {};
    blob_buf_init(&buf, 0);
    blobmsg_add_u32(&buf, "response_code", error_code);
    blobmsg_add_string(&buf, "message", string);
    char *resp = blobmsg_format_json(buf.head, true);
    tuyalink_thing_property_report(&client_instance, args.device_id, resp);
    if(resp != NULL)
        free(resp);
    blob_buf_free(&buf);
}

uint32_t get_ubus_id(char *object){
    uint32_t id;
    if(ubus_lookup_id(uctx, object, &id)){
        syslog(LOG_ERR, "Failed to look up %s object", object);
        return_error_msg(UBUS_STATUS_NOT_FOUND, "Failed to look up ubus object");
        return -1;
    }
    return id;
}

void return_cb(struct ubus_request *req, int type, struct blob_attr *msg){
    if(!msg)
        return;
    struct blob_buf *resp = (struct blob_buf *)req->priv;
    struct blob_attr *cur;
    size_t rem;
    blobmsg_for_each_attr(cur, msg, rem)
        blobmsg_add_blob(resp, cur);
}