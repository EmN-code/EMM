#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include "mqtt.h"
#include "mqtt_client.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_KEEPALIVE 30
#define MQTT_TOPIC "state"

static mqtt_client_t mqtt_client;
static sock_udp_ep_t broker_ep;

/* Callback function for connection to MQTT broker */
static void on_connect(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    (void)client;
    (void)arg;
    if (status == MQTT_CONNECTION_ACCEPTED) {
        printf("Successfully connected to MQTT broker\n");
    } else {
        printf("Connection to MQTT broker failed\n");
    }
}

/* Function to publish a message to the specified MQTT topic */
static void publish_state(void) {
    const char *state_message = "work or not work";
    mqtt_publish(&mqtt_client, 0, MQTT_TOPIC, state_message, strlen(state_message), MQTT_QOS_0, 0);
    printf("Published: %s to topic: %s\n", state_message, MQTT_TOPIC);
}

int main(void) {
    printf("Starting MQTT Example\n");

    /* Initialize the broker endpoint */
    broker_ep.family = AF_INET;
    broker_ep.netif = SOCK_ADDR_ANY_NETIF;
    broker_ep.port = BROKER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&broker_ep.addr, BROKER_IP);

    /* Initialize the MQTT client */
    mqtt_client_init(&mqtt_client, &broker_ep, CLIENT_ID, mqtt_buf, sizeof(mqtt_buf), mqtt_pub_buf, \
                     sizeof(mqtt_pub_buf), on_connect, NULL, mqtt_recv_cb, NULL);

    /* Connect to the MQTT broker */
    mqtt_connect(&mqtt_client, false, MQTT_KEEPALIVE, NULL, NULL, NULL, 0, 0);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    /* Disconnect after use (unreachable in this example, so commented) */
    //mqtt_disconnect(&mqtt_client);

    return 0;
}