/* Necessary imports */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <net/emcute.h>

/* Constants */
#define MQTT_BROKER_IP       "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define PUBLISH_INTERVAL     5
#define EMCUTE_ID            "RIOTNode"
#define EMCUTE_PORT          1883

/* Main Function */
int main(void) {
    /* Declare and initialize variables */
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);
    emcute_topic_t topic;
    int ret;
    const char *state = "work";
    
    /* Initialize the UDP network and connect to the broker */
    ret = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (ret != EMCUTE_OK) {
        printf("Error: Unable to connect to the broker.\n");
        return 1;
    }

    /* Register the topic */
    topic.name = "device/state";
    ret = emcute_reg(&topic);
    if (ret != EMCUTE_OK) {
        printf("Error: Unable to register topic.\n");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (true) {
        ret = emcute_pub(&topic, state, strlen(state), 0);
        if (ret != EMCUTE_OK) {
            printf("Error: Unable to publish data.\n");
        }
        sleep(PUBLISH_INTERVAL);
        /* Toggle state between "work" and "not work" for demonstration */
        if (strcmp(state, "work") == 0) {
            state = "not work";
        } else {
            state = "work";
        }
    }

    return 0;
}