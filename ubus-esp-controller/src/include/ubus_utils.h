#ifndef UBUS_UTILS_H
#define UBUS_UTILS_H

#include "serialport_utils.h"

int check_args(struct ubus_context *ctx, struct ubus_request_data *req, const struct blobmsg_policy *policy, int end, struct blob_attr *args[]);
void return_error_msg(struct ubus_context *ctx, struct ubus_request_data *req, int error_code, char *string);
void build_devices_blob(struct blob_buf *b, struct port ports[], int n);
int reformat_port_info_blob(struct ubus_context *ctx, struct ubus_request_data *req, struct blob_buf *dest, char *buffer);
void return_success_msg(struct ubus_context *ctx, struct ubus_request_data *req, struct blob_buf *data);

#endif