#include "globals.h"
#include "tuya-sdk/tuyalink_core.h"
#include "ubusmsg.h"
#include <syslog.h>
#include "tuya_actions.h"
#include "ubus_functions.h"
#include "ubus_utils.h"
#include <libubox/blobmsg_json.h>

void switch_light(cJSON *params, char *action){
    cJSON *pin = cJSON_GetObjectItem(params, "pin");
    cJSON *on = cJSON_GetObjectItem(params, "on");
    cJSON *port = cJSON_GetObjectItem(params, "port");
    
    if(pin == NULL || on == NULL || port == NULL){
        syslog(LOG_ERR, "Failed to parse action: %s", action);
        return_error_msg(UBUS_STATUS_UNKNOWN_ERROR, "Failed to parse action");
        return;
    }

    uint32_t id;
    if((id = get_ubus_id(UBUS_OBJECT)) == -1)
        return;

    char *string = invoke_light(id, port->valuestring, pin->valueint, on->valueint, 3000);
    tuyalink_thing_property_report(&client_instance, args.device_id, string);
    free(string);
}

void get_devices(char *action){
    uint32_t id;
    if((id = get_ubus_id(UBUS_OBJECT)) == -1)
        return;

    char *string = invoke_empty(id, "devices", 3000);
    tuyalink_thing_property_report(&client_instance, args.device_id, string);
    free(string);
}

void get_sensor(cJSON *params, char *action){
    cJSON *pin = cJSON_GetObjectItem(params, "pin");
    cJSON *model = cJSON_GetObjectItem(params, "model");
    cJSON *sensor = cJSON_GetObjectItem(params, "sensor");
    cJSON *port = cJSON_GetObjectItem(params, "port");

    if(pin == NULL || model == NULL || sensor == NULL || port == NULL){
        syslog(LOG_ERR, "Failed to parse action: %s", action);
        return_error_msg(UBUS_STATUS_UNKNOWN_ERROR, "Failed to parse action");
        return;
    }

    uint32_t id;
    if((id = get_ubus_id(UBUS_OBJECT)) == -1)
        return;

    char *string = invoke_sensor(id, port->valuestring, pin->valueint, model->valuestring, sensor->valuestring, 3000);
    tuyalink_thing_property_report(&client_instance, args.device_id, string);
    free(string);
}