#include "serial_methods.h"
#include "serialport_utils.h"
#include "ubus_utils.h"
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <libserialport.h>
#include <libubus.h>
#include <syslog.h>
#include <ubusmsg.h>

const struct blobmsg_policy toggle_policy[__TOGGLE_MAX] = {
    [TOGGLE_PORT] = { .name = "port", .type = BLOBMSG_TYPE_STRING },
    [TOGGLE_PIN] = { .name = "pin", .type = BLOBMSG_TYPE_INT32 }
};

const struct blobmsg_policy get_policy[__GET_MAX] = {
    [GET_PORT] = { .name = "port", .type = BLOBMSG_TYPE_STRING },
    [GET_PIN] = { .name = "pin", .type = BLOBMSG_TYPE_INT32 },
    [GET_MODEL] = { .name = "model", .type = BLOBMSG_TYPE_STRING },
    [GET_SENSOR] = { .name = "sensor", .type = BLOBMSG_TYPE_STRING }
};

static int toggle_light(struct ubus_context *ctx, struct ubus_request_data *req, struct blob_attr *msg, char *action){
    (void)ctx;
    (void)req;

    struct blob_attr *tb[__TOGGLE_MAX];

    int ret = blobmsg_parse(toggle_policy, __TOGGLE_MAX, tb, blob_data(msg), blob_len(msg));
    if(ret != 0){
        return_error_msg(ctx, req, UBUS_STATUS_UNKNOWN_ERROR, "Failed to parse arguments");
        return UBUS_STATUS_UNKNOWN_ERROR;
    }

    if((ret = check_args(ctx, req, toggle_policy, __TOGGLE_MAX, tb)) != 0)
        return ret;

    struct sp_port *port = set_up_port(blobmsg_get_string(tb[TOGGLE_PORT]));
    if(port == NULL){
        return_error_msg(ctx, req, UBUS_STATUS_CONNECTION_FAILED, "Failed to set up port");
        return UBUS_STATUS_CONNECTION_FAILED;
    }

    char command[MAX_BUFFER_SIZE];
    snprintf(command, MAX_BUFFER_SIZE, LIGHT_COMMAND, action, blobmsg_get_u32(tb[TOGGLE_PIN]));
    if((ret = port_write(port, ctx, req, command, 1000)) != 0)
        return ret;

    return_success_msg(ctx, req, NULL);
    sp_close(port);
    sp_free_port(port);
    return UBUS_STATUS_OK;
}

int turn_on(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg){
    (void)obj;
    (void)method;
    return toggle_light(ctx, req, msg, "on");
}

int turn_off(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg){
    (void)obj;
    (void)method;
    return toggle_light(ctx, req, msg, "off");
}

int get_devices(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg){
    (void)obj;
    (void)method;
    (void)msg;

    int ret;
    int n;
    if((ret = devices_list(esp_ports, &n, MAX_PORTS)) != UBUS_STATUS_OK){
        return_error_msg(ctx, req, ret, "Failed to get device list");
        return ret;
    }

    struct blob_buf resp = {};
    blob_buf_init(&resp, 0);
    build_devices_blob(&resp, esp_ports, n);
    ubus_send_reply(ctx, req, resp.head);
    blob_buf_free(&resp);

    return UBUS_STATUS_OK;
}

int get_info(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg){
    (void)obj;
    (void)method;

    struct blob_attr *tb[__GET_MAX];

    int ret = blobmsg_parse(get_policy, __GET_MAX, tb, blob_data(msg), blob_len(msg));
    if(ret != 0){
        return_error_msg(ctx, req, UBUS_STATUS_UNKNOWN_ERROR, "Failed to parse arguments");
        return UBUS_STATUS_UNKNOWN_ERROR;
    }

    if((ret = check_args(ctx, req, get_policy, __GET_MAX, tb)) != 0)
        return ret;

    struct sp_port *port = set_up_port(blobmsg_get_string(tb[TOGGLE_PORT]));
    if(port == NULL){
        return_error_msg(ctx, req, UBUS_STATUS_CONNECTION_FAILED, "Failed to set up port");
        return UBUS_STATUS_CONNECTION_FAILED;
    }

    char command[MAX_BUFFER_SIZE];
    snprintf(command, MAX_BUFFER_SIZE,  GET_COMMAND, blobmsg_get_string(tb[GET_SENSOR]), blobmsg_get_u32(tb[GET_PIN]), blobmsg_get_string(tb[GET_MODEL]));
    if((ret = port_write(port, ctx, req, command, 1000)) != 0)
        return ret;

    char buffer[MAX_BUFFER_SIZE];
    if((ret = port_read(port, ctx, req, buffer, MAX_BUFFER_SIZE, 2000)) != 0)
        return ret;

    struct blob_buf tmp = {};
    blob_buf_init(&tmp, 0);
    if(reformat_port_info_blob(ctx, req, &tmp, buffer) == UBUS_STATUS_OK)
        return_success_msg(ctx, req, &tmp);
    blob_buf_free(&tmp);

    sp_close(port);
    sp_free_port(port);
    return 0;
}