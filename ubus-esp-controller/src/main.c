#include "serial_methods.h"
#include "serialport_utils.h"
#include <signal.h>

static struct ubus_context *ctx;

static const struct ubus_method serial_methods[] = {
    UBUS_METHOD_NOARG("devices", get_devices),
    UBUS_METHOD("on", turn_on, toggle_policy),
    UBUS_METHOD("off", turn_off, toggle_policy),
    UBUS_METHOD("get", get_info, get_policy)
};

static struct ubus_object_type serial_object_type = UBUS_OBJECT_TYPE("esp-controller", serial_methods);

static struct ubus_object serial_object = {
    .name = "esp-controller",
    .type = &serial_object_type,
    .methods = serial_methods,
    .n_methods = ARRAY_SIZE(serial_methods)
};

void exit_handler(){
    uloop_end();
}

int main(){
    signal(SIGINT, exit_handler);
    signal(SIGTERM, exit_handler);
    signal(SIGQUIT, exit_handler);

    uloop_init();
    ctx = ubus_connect(NULL);
    if(!ctx){
        fprintf(stderr, "Failed to connect to ubus\n");
        return 1;
    }

    ubus_add_uloop(ctx);
    ubus_add_object(ctx, &serial_object);
    uloop_run();
    ubus_free(ctx);
    uloop_done();

    return 0;
}