#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt_client.h"  // Ensure this is the last header if MQTTClient.h is required

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define PUBLISH_INTERVAL_SEC 5

static mqtt_client_t client;

static void on_pub(void *arg, int res)
{
    if (res < 0) {
        printf("Error while publishing: %d\n", res);
    } else {
        printf("Published message\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };

    if (mqtt_client_init(&client, &remote, on_pub, NULL) != MQTT_OK) {
        printf("Failed to initialize mqtt client\n");
        return 1;
    }

    mqtt_connect_params_t params = {
        .client_id = "esp32_client",
    };

    if (mqtt_connect(&client, &params) != MQTT_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0);
        sleep(PUBLISH_INTERVAL_SEC);
    }

    mqtt_disconnect(&client);

    return 0;
}