#ifndef SDK_FUNCTIONS_H
#define SDK_FUNCTIONS_H

#include <tuya-sdk/tuyalink_core.h>
#include <libubus.h>
#include "argument_parser.h"

struct arguments;

int sdk_init(tuya_mqtt_context_t* client);
void sdk_deinit(tuya_mqtt_context_t* client);
int sdk_connect(tuya_mqtt_context_t* client);
void sdk_disconnect(tuya_mqtt_context_t* client);
void sdk_loop(tuya_mqtt_context_t* client, struct ubus_context *ctx);

#endif
