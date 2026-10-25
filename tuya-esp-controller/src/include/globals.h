#ifndef GLOBALS_H
#define GLOBALS_H

#include "argument_parser.h"
#include <linux/limits.h>
#include <stdio.h>
#include <signal.h>
#include <libubus.h>
#include <tuya-sdk/tuyalink_core.h>

extern tuya_mqtt_context_t client_instance;
extern sig_atomic_t g_run;
extern char g_pidpath[PATH_MAX];
extern int g_pidfd;
extern struct ubus_context *uctx;
extern struct arguments args;

#endif
