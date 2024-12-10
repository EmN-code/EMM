#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define MQTT_TOPIC       "device_state"
#define MQTT_MSG         "work"
#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL     (5 * US_PER_SEC)

static emcute_sock_t sock;

int main(void) {
    sock.ipv6 = IPV6_ADDR_ANY;
    sock.port = 0;

    printf("MQTT-SN client on RIOT starting up...\n");

    /* Initialize the emcute socket */
    if (emcute_sock_init(&sock) != EMCUTE_OK) {
        printf("Error: Unable to initialize EMCUTE\n");
        return 1;
    }

    sock.cb = NULL;
    sock.arg = NULL;

    /* Connect to broker */
    ipv6_addr_t broker_addr;

    if (!ipv6_addr_from_str(&broker_addr, MQTT_BROKER_IP)) {
        printf("Error: Invalid broker address\n");
        return 1;
    }

    if (emcute_sock_connect(&sock, &broker_addr, MQTT_BROKER_PORT) != EMCUTE_OK) {
        printf("Error: Unable to connect to broker\n");
        return 1;
    }
    printf("Successfully connected to broker\n");

    char mqtt_msg[] = MQTT_MSG;

    /* Main loop to publish messages */
    while (1) {
        /* Publish the message */
        if (emcute_sock_pub(&sock, MQTT_TOPIC, mqtt_msg, strlen(mqtt_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: Unable to publish message\n");
        } else {
            printf("Message published: %s\n", mqtt_msg);
        }

        /* Wait PUB_INTERVAL seconds */
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    return 0;
}