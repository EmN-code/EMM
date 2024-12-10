#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include "emcute.h"

#define BUF_SIZE          (128U)
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_PORT  (1883U)
#define MQTT_BROKER_IP    "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static char *topic_name = "device/state";

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(MQTT_BROKER_PORT, "device_esp32");
    return NULL;
}

int main(void) {
    /* Initialize the message queue for the thread */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Start emcute in its own thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    /* Configure the MQTT-SN broker address */
    sock_udp_ep_t gw = { .port = MQTT_BROKER_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    /* Connect to the MQTT-SN broker */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to the broker (%d)\n", res);
        return 1;
    }

    /* Setup the Emcute topic data structure */
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = topic_name;

    /* Publish the state every 5 seconds */
    while (1) {
        const char *state = "work or not work";
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data (%d)\n", res);
        }
        xtimer_sleep(5);
    }

    return 0;
}