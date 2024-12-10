#include "netinet/in.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "MQTTClient.h"
#include "esp_system.h"
#include "kerne_defines.h"

#define MQTT_BROKER_IP   "your.mqtt.broker.ip"
#define MQTT_BROKER_PORT 1883

static mqtt_client_t client;
static network_t network;

void mqtt_event_callback(mqtt_client_t *client, mqtt_event_t event, void *user_data) {
    switch (event) {
    case MQTT_CONNECTED:
        printf("MQTT connected\n");
        break;
    case MQTT_DISCONNECTED:
        printf("MQTT disconnected\n");
        break;
    case MQTT_SUBSCRIBED:
        printf("MQTT subscribed\n");
        break;
    case MQTT_UNSUBSCRIBED:
        printf("MQTT unsubscribed\n");
        break;
    case MQTT_PUBLISHED:
        printf("MQTT published\n");
        break;
    case MQTT_DATA:
        printf("MQTT data received\n");
        break;
    case MQTT_ERROR:
        printf("MQTT error occurred\n");
        break;
    default:
        break;
    }
}

void mqtt_connect(void) {
    memset(&client, 0, sizeof(client));
    client.transport.type = MQTT_TRANSPORT_TCP;
    client.callbacks = mqtt_event_callback;
    
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("MQTT network connection failed\n");
            sleep(1);
            continue;
        }

        rc = MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
        if (rc != 0) {
            printf("MQTT client initialization failed\n");
            sleep(1);
            continue;
        }

        mqtt_connect_attr_t conn_attr = {
            .username = "your_username",
            .password = "your_password",
            .keep_alive = 60,
            .client_id = "your_client_id"
        };

        rc = mqtt_connect(&client, &conn_attr);
        if (rc != 0) {
            printf("MQTT connection failed\n");
            sleep(1);
            continue;
        }
        printf("MQTT connection success\n");
        break;
    }
}

int main(void) {
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is an ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    mqtt_connect();
    return 0;
}