#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID_LEN      (64U)
#define PUB_INTERVAL       (5U)
#define SERVER_ADDR        "47.102.103.1"
#define TOPIC_NAME         "state"

static sock_udp_ep_t server;
static emcute_topic_t topic;

int main(void) {
    char topic_name[EMCUTE_ID_LEN] = TOPIC_NAME;
    int res;

    /* setup the server address */
    server.family = AF_INET;
    server.port = EMCUTE_PORT;
    if (inet_pton(AF_INET, SERVER_ADDR, &server.addr.ipv4) != 1) {
        printf("Error: unable to parse IP address\n");
        return 1;
    }

    /* connect to MQTT-SN broker */
    res = emcute_con(&server, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    /* setup the MQTT-SN topic */
    topic.name = topic_name;
    topic.id = 0;

    /* main loop */
    while (1) {
        /* publish "work" state */
        const char *msg = "work";
        res = emcute_pub(&topic, msg, strlen(msg), 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}