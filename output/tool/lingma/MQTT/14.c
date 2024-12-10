#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "mqtt.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define MQTT_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static mqtt_t client;
static char sendbuf[128];
static char readbuf[128];

static void _on_puback(mqtt_client_t *client, void *arg, const mqtt_packet_t *packet) {
    (void)client;
    (void)arg;
    (void)packet;
    printf("Published message acknowledged\n");
}

static void _on_connect(mqtt_client_t *client, void *arg, mqtt_connection_state_t state) {
    (void)client;
    (void)arg;
    if (state == MQTT_CONNECTION_ACCEPTED) {
        printf("MQTT connected\n");
    } else {
        printf("MQTT connection failed\n");
    }
}

static void _on_disconnect(mqtt_client_t *client, void *arg, mqtt_disconnect_reason_t reason) {
    (void)client;
    (void)arg;
    printf("MQTT disconnected: %d\n", reason);
}

static void _on_publish(mqtt_client_t *client, void *arg, const mqtt_packet_t *packet) {
    (void)client;
    (void)arg;
    (void)packet;
    printf("Received message on topic: %.*s\n", packet->topic_len, packet->topic);
    printf("Message: %.*s\n", packet->payload_len, packet->payload);
}

static void _publish_state(mqtt_client_t *client, const char *state) {
    mqtt_message_t message = {
        .qos = MQTT_QOS_0,
        .retain = 0,
        .dup = 0,
        .id = 0,
        .payload = (uint8_t *)state,
        .payload_len = strlen(state),
    };

    mqtt_publish(client, MQTT_TOPIC, &message, _on_puback, NULL);
}

static void *mqtt_task(void *arg) {
    (void)arg;

    mqtt_init(&client, NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_set_connect_handler(&client, _on_connect, NULL);
    mqtt_set_disconnect_handler(&client, _on_disconnect, NULL);
    mqtt_set_publish_handler(&client, _on_publish, NULL);

    mqtt_connect(&client);

    while (1) {
        if (mqtt_is_connected(&client)) {
            _publish_state(&client, MQTT_MESSAGE_WORK);
        } else {
            printf("Not connected, retrying...\n");
            mqtt_connect(&client);
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    puts("Starting MQTT client");

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_task, NULL, "mqtt_task");

    if (pid == KERNEL_PID_UNDEF) {
        puts("Failed to create MQTT task");
        return 1;
    }

    msg_t msg;
    while (1) {
        msg_receive(&msg);
    }

    return 0;
}