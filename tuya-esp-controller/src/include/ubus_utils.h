#ifndef UBUS_UTILS_H
#define UBUS_UTILS_H

#include <libubus.h>

void return_error_msg(int error_code, char *string);
uint32_t get_ubus_id(char *object);
void return_cb(struct ubus_request *req, int type, struct blob_attr *msg);

#endif