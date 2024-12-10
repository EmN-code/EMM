#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"

#define EMCUTE_ID          (1U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define STACKSIZE          (THREAD_STACKSIZE_DEFAULT)
#define SERVER_IP          "47.102.103.1"
#define SERVER_PORT        1883

static char stack[STACKSIZE];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static void *emcute_thread(void *arg) {
    (void)arg;

    while (1) {
        /* Publish "work" or "not work" here */
        const char *data = "work";
        int res = emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Publishing %s failed with error code %d\n", data, res);
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to the gateway: %d\n", res);
        return 1;
    }

    /* Register and initialize the MQTT-SN topic */
    topic.name = "state";
    topic.id = 1;

    /* Create the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}