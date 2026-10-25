# tuya-rutx-esp-controller

Control ESP microcontrollers connected over USB to a Teltonika RUTX router (OpenWrt / RutOS) from the Tuya Cloud. Tuya actions are translated into `ubus` calls, which in turn talk to the ESP over a serial port. The cloud connection uses the [Tuya IoT Core SDK for Embedded C](https://github.com/tuya/tuya-iot-core-sdk).

```
Tuya Cloud  <--MQTT-->  tuya-esp-controllerd  <--ubus-->  esp-controller  <--USB serial-->  ESP
```

## Components

| Package                   | Description                                                                          |
| ------------------------- | ------------------------------------------------------------------------------------ |
| `esp-controller`          | `ubus` service (`esp-controller` object) that talks to the ESP over a serial port    |
| `tuya-esp-controller`     | Daemon that connects to the Tuya Cloud, receives actions and calls `esp-controller`  |
| `tuya-sdk`                | OpenWrt package for the Tuya IoT Core SDK                                            |
| `libserialport`           | OpenWrt package for [libserialport](https://sigrok.org/wiki/Libserialport)           |
| `tuya-esp-controller-api` | VuCI API support for configuring the service from the web UI                         |

## esp-controller (ubus)

The service looks for USB serial devices with vendor ID `0x10C4` and product ID `0xEA60` (Silicon Labs CP210x). Commands are sent to the ESP as JSON over serial.

| Method    | Arguments                          | Description                               |
| --------- | ---------------------------------- | ----------------------------------------- |
| `devices` | none                               | List connected ESP devices                |
| `on`      | `port`, `pin`                      | Turn a pin on                             |
| `off`     | `port`, `pin`                      | Turn a pin off                            |
| `get`     | `port`, `pin`, `model`, `sensor`   | Read a sensor (temperature and humidity)  |

```sh
ubus call esp-controller devices
ubus call esp-controller on '{"port": "/dev/ttyUSB0", "pin": <pin>}'
ubus call esp-controller get '{"port": "/dev/ttyUSB0", "pin": <pin>, "model": "<model>", "sensor": "<sensor>"}'
```

## Tuya actions

`tuya-esp-controllerd` handles the following actions defined on your Tuya product. The result of each action is sent back to the cloud as a property report.

| Action code | Parameters                       | Description                |
| ----------- | -------------------------------- | -------------------------- |
| `light`     | `port`, `pin`, `on` (0 or 1)     | Switch a pin on or off     |
| `get`       | `port`, `pin`, `model`, `sensor` | Read a sensor              |
| `devices`   | none                             | List connected ESP devices |

## Building

Copy the directories of this repository into your OpenWrt / RutOS SDK, select `tuya-esp-controller` (Base system) in `make menuconfig` and build:

```sh
make package/tuya-esp-controller/compile
```

All other packages, including `esp-controller`, should be pulled in as dependencies.

## Configuration

Create a device in the [Tuya IoT Platform](https://iot.tuya.com/) and set its credentials in the config file which should have been automatically generated during the installation at `/etc/config/tuya-esp-controllerd`:


```
config tuya-esp-controllerd 'device_sct'
	option device_id 'YOUR_DEVICE_ID'
	option product_id 'YOUR_PRODUCT_ID'
	option secret 'YOUR_DEVICE_SECRET'
	option enable '1'
```

The services are managed by `procd`, start on boot and restart automatically if they exit. The Tuya daemon restarts whenever its config changes.

## Running manually

```sh
tuya-esp-controllerd -p <PRODUCT_ID> -d <DEVICE_ID> -s <DEVICE_SECRET> [-D]
```

Logs go to syslog:

```sh
logread | grep tuya_esp_controllerd
```