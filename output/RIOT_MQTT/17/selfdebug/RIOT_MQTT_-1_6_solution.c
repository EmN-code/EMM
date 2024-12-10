#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/netdev.h>
#include <net/af.h>
#include <net/mqtt.h>  // Include the correct MQTT header

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static msg_t msg_queue[8];

void mqtt_callback(mqtt_client_t *client, const mqtt_evt_t *evt, void *data) {
    (void)client;
    (void)data;

    switch (evt->type) {
        case MQTT_EVT_CONNECT:
            printf("MQTT: connected to broker\n");
            break;
        case MQTT_EVT_DISCONNECT:
            printf("MQTT: disconnected from broker\n");
            break;
        default:
            break;
    }
}

int main(void) {
    // Init message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    // Start connecting to the broker
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    mqtt_client_t client;
    mqtt_client_init(&client, &remote, mqtt_callback, NULL);
    
    int res = mqtt_connect(&client, false, NULL, NULL, 0, NULL, 0, 0);
    if (res != 0) {
        printf("Error connecting to MQTT broker\n");
        return 1;
    }

    // Publish messages every 5 seconds
    while (1) {
        const char *msg = "work";
        mqtt_publish(&client, "state_topic", msg, strlen(msg), MQTT_QOS_0, false);

        // Wait for the next publish interval
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}