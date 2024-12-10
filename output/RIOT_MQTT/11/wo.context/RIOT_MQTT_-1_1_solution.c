#include <stdlib.h>
#include <stdio.h>
#include <net/emcute_sock.h>
#include <net/emcute.h>
#include <string.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID   "esp32_mqtt_client"

static emcute_topic_t topic;

void on_pub(const char *topic, void *data, size_t len) {
    (void) topic;
    (void) data;
    (void) len;
}

int mqtt_connect_and_publish(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .netif = NETIF_IPV6_NETIF_NONE };
    char *topic_name = "stateTopic";
    topic.name = topic_name;
    
    /* Setup gateway endpoint */
    if (inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6) == 0) {
        printf("Error setting gateway address\n");
        return 1;
    }
    gw.port = SERVER_PORT;

    /* Connect to MQTT broker */
    if (emcute_con(&gw, true, CLIENT_ID, NULL, NULL, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%i\n", SERVER_ADDR, SERVER_PORT);
        return 1;
    }
    printf("Connected to broker\n");

    while(1) {
        /* Publish data every 5 seconds */
        char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_sleep(5);
    }
    return 0;
}

int main(void) {
    printf("Starting ESP32 MQTT client example\n");
    mqtt_connect_and_publish();
    return 0;
}