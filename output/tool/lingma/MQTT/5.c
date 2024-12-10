#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static mqtt_client_t client;
static char send_buf[128];
static char read_buf[128];

static void _on_puback(mqtt_client_t *client, void *arg, const mqtt_packet_t *puback) {
    (void)client;
    (void)arg;
    (void)puback;
    printf("Published message acknowledged\n");
}

static void _on_connect(mqtt_client_t *client, void *arg, mqtt_connection_state_t state) {
    (void)arg;
    if (state == MQTT_CS_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else {
        printf("Failed to connect to MQTT broker\n");
    }
}

static void _publish_state(mqtt_client_t *client, const char *state) {
    mqtt_topic_t topic = MQTT_TOPIC;
    mqtt_message_t message = {
        .qos = MQTT_QOS_AT_MOST_ONCE,
        .retain = false,
        .payload = (uint8_t *)state,
        .payload_len = strlen(state)
    };

    mqtt_publish(client, &topic, &message, _on_puback, NULL);
}

static void *mqtt_task(void *arg) {
    (void)arg;

    mqtt_client_init(&client, NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, send_buf, sizeof(send_buf), read_buf, sizeof(read_buf));

    mqtt_event_callback_set(&client, MQTT_EV_CONNECT, _on_connect, NULL);
    mqtt_event_callback_set(&client, MQTT_EV_PUBACK, _on_puback, NULL);

    mqtt_connect(&client);

    while (1) {
        if (mqtt_is_connected(&client)) {
            _publish_state(&client, MQTT_MESSAGE_WORK);
            xtimer_sleep(5); // Sleep for 5 seconds
        } else {
            printf("Not connected, retrying...\n");
            mqtt_connect(&client);
            xtimer_sleep(1); // Sleep for 1 second before retrying
        }
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT client...\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_task, NULL, "mqtt_task");

    while (1) {
        xtimer_sleep(1); // Main loop to keep the program running
    }

    return 0;
}