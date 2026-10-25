#include "ubus_functions.h"
#include "ubus_utils.h"
#include "globals.h"
#include <libubus.h>
#include <libubox/blobmsg_json.h>

char *invoke_sensor(int id, char *port, int pin, char *model, char *sensor, int timeout){
    struct blob_buf req = {};
    struct blob_buf resp = {};
    blob_buf_init(&req, 0);
    blob_buf_init(&resp, 0);

    blobmsg_add_string(&req, "port", port);
    blobmsg_add_u32(&req, "pin", pin);
    blobmsg_add_string(&req, "model", model);
    blobmsg_add_string(&req, "sensor", sensor);

    int ret = ubus_invoke(uctx, id, "get", req.head, return_cb, (void *)&resp, timeout);
    char *string = blobmsg_format_json(resp.head, true);

    blob_buf_free(&resp);
    blob_buf_free(&req);
    return string;
}

char *invoke_light(int id, char *port, int pin, int mode, int timeout){
    struct blob_buf req = {};
	blob_buf_init(&req, 0);
	blobmsg_add_string(&req, "port", port);
    blobmsg_add_u32(&req, "pin", pin);
    int ret = -1;

    struct blob_buf resp = {};
    blob_buf_init(&resp, 0);

    if(mode == 0){
        ret = ubus_invoke(uctx, id, "off", req.head, return_cb, (void *)&resp, timeout);
    }
    else if (mode == 1){
        ret = ubus_invoke(uctx, id, "on", req.head, return_cb, (void *)&resp, timeout);
    }
    else{
        return_error_msg(UBUS_STATUS_INVALID_ARGUMENT, "Invalid toggle option");
        blob_buf_free(&req);
        blob_buf_free(&resp);
        return NULL;
    }

    char *string = blobmsg_format_json(resp.head, true);

    blob_buf_free(&req);
    blob_buf_free(&resp);
    return string;
}

char *invoke_empty(int id, char *action, int timeout){
    struct blob_buf resp = {};
    blob_buf_init(&resp, 0);

    int ret = ubus_invoke(uctx, id, action, NULL, return_cb, (void *)&resp, timeout);
    char *string = blobmsg_format_json(resp.head, true);

    blob_buf_free(&resp);
    return string;
}