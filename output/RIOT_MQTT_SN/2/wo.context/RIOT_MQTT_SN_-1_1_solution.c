#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/netif.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U)  // Publish every 5 seconds
#define TOPIC_NAME "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

/**
 * @brief Callback function for handling received MQTT-SN messages.
 */
static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    char *msg = (char *)data;
    msg[len] = '\0';
    printf("Received: %s\n", msg);
}

void *publisher_thread(void *arg)
{
    (void)arg;
    char *states[2] = {"work", "not work"};
    int state_index = 0;

    while (1) {
        state_index = !state_index;

        emcute_msg_t msg;
        msg.data = (void *)states[state_index];
        msg.len = strlen(states[state_index]);
        msg.flags = EMCUTE_MSG_FLAG_QOS_0;

        if (emcute_pub(&topic, &msg) != EMCUTE_OK) {
            printf("Error: unable to publish data to topic '%s'\n", TOPIC_NAME);
        }

        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000U);
    }

    return NULL;
}

int main(void)
{
    printf("RIOT MQTT-SN Example\n");

    /* initialize network interface */
    netif_t *netif = gnrc_netif_iter(NULL);
    if (netif == NULL) {
        printf("Error: No network interface found\n");
        return 1;
    }

    /* setup MQTT-SN client */
    emcute_run(netif, NULL, EMCUTE_PORT);

    /* connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4) == 0) {
        printf("Error: Unable to parse broker address\n");
        return 1;
    }
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to MQTT-SN broker\n");
        return 1;
    }
    printf("Connected to MQTT-SN broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    /* setup topic structure */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* create the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}