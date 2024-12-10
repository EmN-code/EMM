
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <stdbool.h>
#include <mma8x5x.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <thread.h>
#include <mutex.h>
#include <mma8x5x_regs.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static emcute_topic_t topic;
static sock_udp_ep_t gw;

void publish_state(void) {
    const char *state = "work or not work";
    int res;

    while (1) {
        res = emcute_pub(&topic, state, strlen(state), 0);
        if (res != EMCUTE_OK) {
            printf("Error publishing state: %d\n", res);
        } else {
            printf("State published\n");
        }

        thread_sleep(PUBLISH_INTERVAL);
    }
}

int main(void) {
    sock_udp_t sock;
    int res;

    /* Initialize the topic structure */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* Set up the gateway endpoint */
    gw.family = AF_INET;
    gw.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    /* Create a UDP socket */
    res = sock_udp_create(&sock, NULL, &gw, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    /* Connect to the MQTT-SN gateway */
    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    /* Register the topic */
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    /* Start the publish thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, (thread_task_func_t)publish_state, NULL, "publish_state");

    return 0;
}
