#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define CLIENT_ID "riot-esp32"

static mqtt_client_t mqtt_client;
static char send_buf[128];
static char recv_buf[128];

static const char *topic = "device/state";
static const char *message = "work";

void timer_callback(void)
{
    mqtt_publish(&mqtt_client, topic, message, strlen(message), MQTT_QOS_0, 0);
    printf("Published message: %s\n", message);
}

int main(void)
{
    printf("Setting up MQTT client...\n");
    mqtt_client_config_t config = {
        .client_id = CLIENT_ID,
        .user = NULL,  // Set this if your broker requires a username
        .pass = NULL,  // Set this if your broker requires a password
        .will_topic = NULL,
        .will_message = NULL
    };

    mqtt_client_init(&mqtt_client, &config, send_buf, sizeof(send_buf), recv_buf, sizeof(recv_buf), MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (mqtt_connect(&mqtt_client) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    printf("MQTT client connected. Starting to publish messages every 5 seconds...\n");
    ztimer_set(ZTIMER_MSEC, PUB_INTERVAL, timer_callback, NULL);
    
    while (1) {
        thread_yield();
    }
    return 0;
}