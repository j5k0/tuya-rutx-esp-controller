#ifndef SERIALPORT_UTILS_H
#define SERIALPORT_UTILS_H

#include <libubox/blobmsg.h>
#include <libserialport.h>
#include <libubus.h>
#include <libubox/blobmsg.h>
#include <libubox/blob.h>
#include <ubusmsg.h>

#define MAX_BUFFER_SIZE 4096
#define MAX_PORTS 64

struct port {
    char port[128];
    char serial_id[64];
    int vendor_id;
    int product_id;
};

extern struct port esp_ports[MAX_PORTS];

struct sp_port* set_up_port(char *port_name);
int devices_list(struct port esp_ports[], int *n, int max);
int port_write(struct sp_port *port, struct ubus_context *ctx, struct ubus_request_data *req, char *string, int timeout);
int port_read(struct sp_port *port, struct ubus_context *ctx, struct ubus_request_data *req, char *buffer, int n, int timeout);

#endif