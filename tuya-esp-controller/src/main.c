#include <signal.h>
#include <syslog.h>
#include <libubus.h>

#include "argument_parser.h"
#include "daemonizer.h"
#include "globals.h"
#include "sdk_functions.h"

#include <tuya-sdk/tuya_error_code.h>

static void cleanup(tuya_mqtt_context_t* client, struct ubus_context *uctx);
static void signal_handler();

sig_atomic_t g_run = 1;
char g_pidpath[PATH_MAX];
int g_pidfd;
struct arguments args;
struct ubus_context *uctx;

tuya_mqtt_context_t client_instance;

int main(int argc, char** argv)
{
    signal(SIGTERM, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGABRT, signal_handler);

    args.daemon = 0;
    args.device_id = NULL;
    args.product_id = NULL;
    args.secret = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if(args.device_id == NULL || args.product_id == NULL || args.secret == NULL)
       return 1;

    openlog("tuya_esp_controllerd", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Starting program");

    tuya_mqtt_context_t* client = &client_instance;
    int ret;

    syslog(LOG_DEBUG, "Connecting to ubus");

    uctx = ubus_connect(NULL);

    if(!uctx){
        syslog(LOG_ERR, "Failed to connect to ubus");
    }

    if(args.daemon){
        if(daemonize() == 1)
            cleanup(client, uctx);
    }

    ret = sdk_init(client);
    if(ret != OPRT_OK){
        cleanup(client, uctx);
        return ret;
    }

    ret = sdk_connect(client);
    if(ret != OPRT_OK){
        cleanup(client, uctx);
        return ret;
    }

    sdk_loop(client, uctx);

    cleanup(client, uctx);

    return ret;
}

static void cleanup(tuya_mqtt_context_t* client, struct ubus_context *uctx){
    int ret;

    syslog(LOG_INFO, "Entering program cleanup");

    sdk_disconnect(client);
    sdk_deinit(client);

    closelog();

    if(uctx != NULL)
        ubus_free(uctx);

    if(args.daemon && g_pidfd != -1){
        syslog(LOG_DEBUG, "Unlocking and removing pid file: %s", g_pidpath);
        if(close(g_pidfd) == -1)
            syslog(LOG_ERR, "Error while trying to close pid file: %s", g_pidpath);
        if(unlink(g_pidpath) == -1)
            syslog(LOG_ERR, "Error while trying to remove pid file: %s", g_pidpath);
    }

    syslog(LOG_INFO, "Program end");
}

static void signal_handler(){
    g_run = 0;
}
