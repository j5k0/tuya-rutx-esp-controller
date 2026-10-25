#ifndef TUYA_ACTIONS_H
#define TUYA_ACTIONS_H

#include <tuya-sdk/cJSON.h>

#define UBUS_OBJECT "esp-controller"

void switch_light(cJSON *params, char *action);
void get_sensor(cJSON *params, char *action);
void get_devices(char *action);

#endif