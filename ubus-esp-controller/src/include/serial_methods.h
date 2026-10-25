#ifndef SERIAL_METHODS_H
#define SERIAL_METHODS_H

#define ESP_PRODUCT_ID 60000
#define ESP_VENDOR_ID 4292

#define LIGHT_COMMAND "{ \"action\": \"%s\", \"pin\": %d }"
#define GET_COMMAND "{ \"action\": \"get\", \"sensor\": \"%s\", \"pin\": %d, \"model\": \"%s\" }"

#include "serialport_utils.h"
#include <libserialport.h>
#include <libubus.h>
#include <libubox/blobmsg.h>
#include <libubox/blob.h>
#include <ubusmsg.h>

enum {
    TOGGLE_PORT,
    TOGGLE_PIN,
    __TOGGLE_MAX
};

enum {
    GET_PORT,
    GET_PIN,
    GET_MODEL,
    GET_SENSOR,
    __GET_MAX
};

extern const struct blobmsg_policy toggle_policy[__TOGGLE_MAX];
extern const struct blobmsg_policy get_policy[__GET_MAX];

int turn_on(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg);
int turn_off(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg);
int get_info(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg);
int get_devices(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg);

#endif