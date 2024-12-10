#include <stdio.h>
#include <string.h>
#include <time.h>
#include <net/mqtt.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"

static mqtt_client_t mqtt_client;
static char mqtt_send_buf[128];
static char mqtt_recv_buf[128];
static mqtt_connect_t connect_params;
static mqtt_publish_t publish_params;

static void *mqtt_thread(void *arg) {
    (void)arg;
    while (1) {
        char *state = STATE_WORK;  // Assuming device is always in "work" state
        publish_params.topic = TOPIC;
        publish_params.message = state;
        publish_params.message_length = strlen(state);

        if (mqtt_publish(&mqtt_client, &publish_params) != MQTT_OK) {
            printf("[MQTT] Error while publishing data\n");
        } else {
            printf("[MQTT] Message published: %s\n", state);
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("[MQTT] Starting MQTT example\n");

    mqtt_client.netcontext.type = SOCK_STREAM;
    connect_params.client_id = CLIENT_ID;
    connect_params.keep_alive = 60;

    sock_udp_ep_t broker_endpoint = {
        .family = AF_INET,
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&broker_endpoint.addr.ipv6, BROKER_IP);

    if (mqtt_connect(&mqtt_client, &broker_endpoint, &connect_params) != MQTT_OK) {
        printf("[MQTT] Unable to connect\n");
        return 1;
    }

    printf("[MQTT] Successfully connected to the broker\n");

    mqtt_thread(NULL);

    return 0;
}
