#include "serial_methods.h"
#include <libserialport.h>
#include "ubus_utils.h"
#include <libserialport.h>
#include <libubox/blob.h>
#include <libubox/blobmsg.h>
#include <libubox/blob.h>
#include <libubus.h>
#include <ubusmsg.h>

struct port esp_ports[MAX_PORTS];

int devices_list(struct port esp_ports[], int *n, int max){
    struct sp_port **port_list;
    enum sp_return result = sp_list_ports(&port_list);
    if(result != SP_OK)
        return UBUS_STATUS_UNKNOWN_ERROR;

    *n=0;
    for(int i=0; port_list[i] != NULL && *n < max; i++){
        struct port tmp;
        struct sp_port *port = port_list[i];
        int vendor, product;
        sp_get_port_usb_vid_pid(port, &vendor, &product);
        if(product != ESP_PRODUCT_ID || vendor != ESP_VENDOR_ID)
            continue;
        char *serial = sp_get_port_usb_serial(port);
        char *port_name = sp_get_port_name(port);
        if(port_name != NULL)
            strncpy(tmp.port, port_name, sizeof(tmp.port));
        if(serial != NULL)
            strncpy(tmp.serial_id, serial, sizeof(tmp.serial_id));
        tmp.product_id = product;
        tmp.vendor_id = vendor;

        esp_ports[*n] = tmp;
        *n = *n + 1;
    }

    sp_free_port_list(port_list);
    return UBUS_STATUS_OK;
}

static int esp_exists(char *port_name){
    int n;
    if(devices_list(esp_ports, &n, MAX_PORTS) != UBUS_STATUS_OK)
        return -1;

    for(int i=0; i<n; i++){
        if(strcmp(port_name, esp_ports[i].port) == 0)
            return 1;
    }
    return 0;
}

struct sp_port* set_up_port(char *port_name){
    struct sp_port *port;
    if(!esp_exists(port_name))
        return NULL;
    sp_get_port_by_name(port_name, &port);
    if(port != NULL){
        sp_open(port, SP_MODE_READ_WRITE);
        sp_set_baudrate(port, 9600);
        sp_set_bits(port, 8);
        sp_set_parity(port, SP_PARITY_NONE);
        sp_set_stopbits(port, 1);
        sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
    }
    return port;
}

int port_write(struct sp_port *port, struct ubus_context *ctx, struct ubus_request_data *req, char *string, int timeout){
    char error_buf[MAX_BUFFER_SIZE];
    int ret = sp_blocking_write(port, string, strlen(string), timeout);
    ret = sp_drain(port);
    if(ret != SP_OK){
        snprintf(error_buf, sizeof(error_buf), "Failed to send action port: %s", sp_get_port_name(port));
        return_error_msg(ctx, req, UBUS_STATUS_TIMEOUT, error_buf);
        sp_close(port);
        sp_free_port(port);
        return UBUS_STATUS_TIMEOUT;
    }
    return 0;
}

int port_read(struct sp_port *port, struct ubus_context *ctx, struct ubus_request_data *req, char *buffer, int n, int timeout){
    char error_buf[MAX_BUFFER_SIZE];
    int ret = sp_blocking_read(port, buffer, n, timeout);
    if(ret <= 0){
        snprintf(error_buf, sizeof(error_buf), "Failed to read response port: %s", sp_get_port_name(port));
        return_error_msg(ctx, req, UBUS_STATUS_TIMEOUT, error_buf);
        sp_close(port);
        sp_free_port(port);
        return UBUS_STATUS_TIMEOUT;
    }
    return 0;
}