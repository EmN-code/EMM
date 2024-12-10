#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xtimer.h>
#include <timex.h>
#include <thread.h>

#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define INTERVAL        (5U * US_PER_SEC)

#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC      "state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

/* Callback function executed when a message is published */
static void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    /* Use the callback function here, if needed. */
}

/* Initialize connection to the MQTT-SN broker */
static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    /* Convert server address from string to byte-represented IP address */
    if (inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6) != 1) {
        printf("Error: unable to parse server address\n");
        return 1;
    }

    /* Set clean session flag and attempt to connect repeatedly until successful */
    unsigned flags = EMCUTE_CS;
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, flags);
        if (res == EMCUTE_OK) {
            printf("Successfully connected to gateway at %s\n", SERVER_ADDR);
            break;
        } else {
            printf("Connection failed: retrying in 1 second\n");
            xtimer_sleep(1);
        }
    }
    return 0;
}

int main(void) {
    /* Initialize emcute */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    memset(stack, 0, sizeof(stack));
    emcute_run(EMCUTE_PORT, "device");

    /* Connect to MQTT-SN broker */
    if (connect_to_broker() != 0) {
        puts("Failed to connect to broker, exiting.");
        return 1;
    }

    /* Prepare MQTT-SN topic */
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;
    emcute_reg(&topic);

    while (1) {
        /* Publish message every five seconds */
        const char *message = "work";
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);

        if (res != EMCUTE_OK) {
            printf("Publish failed with result code %d
", res);
        } else {
            printf("Message published: %s
", message);
        }

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    /* Disconnect (not strictly reachable code as this runs indefinitely) */
    emcute_discon();
    return 0;
}