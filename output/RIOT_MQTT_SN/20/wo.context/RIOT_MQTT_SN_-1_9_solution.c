#include <thread.h>
#include <emcute.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC) // 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

void *mqtt_pub_thread(void *arg) {
    (void)arg;

    while (1) {
        const char *data = "work or not work";
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC); // Sleep for the interval
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* setup our subscription topic */
    topic.name = "state";
    topic.id = 0; // will be assigned by the broker

    /* initialize the network */
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse broker address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_ADDRESS, BROKER_PORT);
        return 1;
    }

    /* start the publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  mqtt_pub_thread, NULL, "mqtt_pub");

    /* keep main thread alive */
    while (1) {
        thread_yield();
    }

    return 0;
}