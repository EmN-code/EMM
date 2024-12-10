#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <net/mqttsn.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <thread.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUB_INTERVAL (5 * US_PER_SEC)

static char* topics[1] = {"state"};
static char json_payload[128];

enum work_state { WORK, NOT_WORK };
static enum work_state current_state = WORK;

void on_message_received(mqttsn_context_t *client, const char *topic, const char *msg, size_t len)
{
    (void)client;
    printf("Received message '%.*s' on topic '%s'\n", (int)len, msg, topic);
}

void publish_state(mqttsn_context_t *client)
{
    if (current_state == WORK) {
        strcpy(json_payload, "{\"state\": \"work\"}");
        current_state = NOT_WORK;
    } else {
        strcpy(json_payload, "{\"state\": \"not work\"}");
        current_state = WORK;
    }
    int result = mqttsn_publish(client, "state", json_payload, strlen(json_payload), 0);
    if (result < 0) {
        printf("Failed to publish state, error: %d\n", result);
    } else {
        printf("State published: %s\n", json_payload);
    }
}

void *mqtt_pub_thread(void *arg)
{
    (void)arg;
    mqttsn_context_t client;
    mqttsn_init(&client, topics, 1, on_message_received);
    int result = mqttsn_connect(&client, SERVER_IP, SERVER_PORT);

    if (result < 0) {
        printf("MQTT connection failed: %d\n", result);
        return NULL;
    }

    while (1) {
        publish_state(&client);
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  mqtt_pub_thread, NULL, "mqtt_pub_thread");
    return 0;
}