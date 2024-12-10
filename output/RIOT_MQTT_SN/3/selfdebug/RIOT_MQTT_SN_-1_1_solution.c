// Necessary header files in descending order
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "net/ipv6/addr.h"
#include "emcute.h"

// Parameters for the connection
#define SERVER_ADDR           "47.102.103.1"
#define SERVER_PORT           1883
#define PUB_INTERVAL          (5U * 1000000U) // Publish every 5 seconds
#define MQTT_TOPIC            "esp32/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;
static const char *status_msgs[] = { "work", "not work" };

/* Function to connect to MQTT-SN broker */
static int connect_broker(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    gw.addr.ipv4[0] = 47; gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103; gw.addr.ipv4[3] = 1;

    if (emcute_con(&gw, true, NULL, NULL, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", SERVER_ADDR, SERVER_PORT);
        return 1;
    }
    printf("Connected to [%s]:%d\n", SERVER_ADDR, SERVER_PORT);

    /* Setup MQTT-SN topic */
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic \"%s\"\n", MQTT_TOPIC);
        return 1;
    }

    return 0;
}

/* Main function of the thread to publish state */
static void *pub_state(void *arg) {
    (void)arg;
    unsigned state = 0;

    while (1) {
        char *status = (char *)status_msgs[state];
        if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published: %s\n", status);
        }

        state = (state + 1) % 2;
        xtimer_usleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    /* Initialize the thread queue */
    msg_init_queue(queue, 8);

    if (connect_broker() != 0) {
        return 1;
    }

    /* Start the publishing thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  pub_state, NULL, "pub_state");

    /* Keeps the main thread alive to let the other threads run */
    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}