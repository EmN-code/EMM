#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h> // For timing functionalities
#include <net/af.h>
#include <net/sock/udp.h>
#include "net/netif.h"
#include "net/mqtt.h"
#include "net/mqttsn.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT    1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds
#define TOPIC "device/state"

static mqtt_client_t client;
static mqtt_client_config_t config;
static msg_t queue[4];

static char send_buf[128];
static char recv_buf[128];

static void on_pub(void *arg, const char *topic, const void *data, size_t len) {
    (void)arg;  // unused
    printf("Published data: %.*s\n", (int)len, (const char *)data);
}

int main(void) {
    puts("RIOT ESP32 MQTT example");

    config.client_id = "esp32_client";
    config.user = NULL;
    config.pass = NULL;

    mqtt_client_init(&client, &config, mqtt_event_handler);
    mqtt_client_register(&client, &queue, queue, 4);

    printf("Connecting to MQTT broker %s\n", BROKER_ADDRESS);
    if (mqtt_client_connect(&client, BROKER_ADDRESS, BROKER_PORT, 0) != 0) {
        puts("Error: Unable to connect to MQTT broker");
        return 1;
    }

    puts("Connection successful");

    while (1) {
        const char *state = "work or not work";  // Determined by some logic
        if (mqtt_client_publish(&client, 0, TOPIC, state, strlen(state), 2) != 0) {
            puts("Error: Unable to publish message");
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    return 0;
}
