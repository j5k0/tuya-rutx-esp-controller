#ifndef UBUS_FUNCTIONS_H
#define UBUS_FUNCTIONS_H

char *invoke_sensor(int id, char *port, int pin, char *model, char *sensor, int timeout);
char *invoke_light(int id, char *port, int pin, int mode, int timeout);
char *invoke_empty(int id, char *action, int timeout);

#endif