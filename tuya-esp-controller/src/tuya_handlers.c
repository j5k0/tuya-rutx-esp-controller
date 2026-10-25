
#include <tuya-sdk/cJSON.h>
#include <tuya-sdk/tuyalink_core.h>

#include <syslog.h>

#include "tuya_handlers.h"
#include "tuya_actions.h"
#include "ubus_utils.h"

void on_connected(tuya_mqtt_context_t* context, void* user_data){
    syslog(LOG_INFO, "Connected to Tuya cloud");
}

void on_disconnect(tuya_mqtt_context_t* context, void* user_data){
    syslog(LOG_INFO, "Disconnected from Tuya cloud");
}

static void parse_tuya_action(char *json){
    cJSON *data = cJSON_Parse(json);
    if(data != NULL){
        cJSON *params = cJSON_GetObjectItem(data, "inputParams");
        cJSON *action = cJSON_GetObjectItem(data, "actionCode");
        if(strcmp(action->valuestring, "light") == 0)
            switch_light(params, action->valuestring);
        else if(strcmp(action->valuestring, "get") == 0)
            get_sensor(params, action->valuestring);
        else if(strcmp(action->valuestring, "devices") == 0)
            get_devices(action->valuestring);
        else
            return_error_msg(UBUS_STATUS_NOT_FOUND, "Invalid action");
        
        cJSON_Delete(data);
    }
    else{
        syslog(LOG_ERR, "Failed to parse json %s", json);
    }
}

void on_messages(tuya_mqtt_context_t* context, void* user_data, const tuyalink_message_t* msg){
    syslog(LOG_INFO, "Received message id: %s type: %d code %d", msg->msgid, msg->type, msg->code);
    switch (msg->type) {
        case THING_TYPE_ACTION_EXECUTE:
            syslog(LOG_INFO, "Action message msgid: %s msg: %s", msg->msgid, msg->data_string);
            parse_tuya_action(msg->data_string);
            break;
        default:
            break;
    }
    printf("\r\n");
}