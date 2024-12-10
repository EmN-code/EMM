#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/mqtt.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "timex.h"
#include "xtimer.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MAIN_QUEUE_SIZE 8
#define MQTT_TOPIC "state"

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static mqtt_client_t mqtt_client;
static mqtt_pkt_t mqtt_pkt;&pkt
static sock_udp_ep_t remote = { .family = AF_INET6, .addr = BROKER_IP, .port = BROKER_PORT };

void on_conack(void *arg, uint8_t result) {
    (void)arg;
    printf("MQTT server responded with CONNACK result code %d\n", result);
    if (result == MQTT_CONN_OK) {
        printf("Successfully connected to the broker!\n");
    }
    else {
        printf("Failed to connect to the broker, result code %d\n", result);
    }
}

void on_puback(void *arg, uint16_t pkt_id, int status) {
    (void)arg;
    printf("MQTT server responded to PUBACK with result code %d for packet %d\n", status, pkt_id);
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    while (1) {
        const char *state = "work";

        mqtt_pkt_init(&mqtt_pkt, MQTT_PKT_PUBLISH, MQTT_DEPENDENCIES_NONE);
        mqtt_pkt_set_publish(&mqtt_pkt, 0, MQTT_QoS_0, 0, MQTT_TOPIC, (const uint8_t *)state, strlen(state) + 1);

        printf("Publishing '%s' to '%s'\n", state, MQTT_TOPIC);

        int res = mqtt_pub_pkt(&mqtt_client, &mqtt_pkt);
        if (res != 0) {
            printf("Failed to publish: %d\n", res);
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mqtt_client_init(&mqtt_client, _main_msg_queue, MAIN_QUEUE_SIZE);

    printf("Connecting to the MQTT broker...\n");

    mqtt_client_con(&mqtt_client, &remote, NULL, NULL, 0, on_conack, NULL, NULL, 0);

    if (mqtt_client_run(&mqtt_client) < 0) {
        puts("Error starting the MQTT client\n");
        return 1;
    }
    
    printf("MQTT client running\n");

    if (thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt") <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    return 0;
}