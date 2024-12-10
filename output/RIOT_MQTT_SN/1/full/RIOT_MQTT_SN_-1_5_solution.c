#include <ztimer.h>
#include <timex.h>
#include <stdatomic.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLICATION_INTERVAL (5U)

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

static int connect_to_gateway(void) {
    ipv4_addr_t addr;
    if (inet_pton(AF_INET, SERVER_ADDR, &addr) == 1) {
        memcpy(&gw.addr.ipv4, &addr, sizeof(addr));
    }
    else {
        return EMCUTE_NOGW;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: Could not connect to gateway.\n");
        return res;
    }

    return EMCUTE_OK;
}

static void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *message = "work";
        int res = emcute_pub(&topic, message, strlen(message), 0);
        if (res != EMCUTE_OK) {
            printf("Error: Could not publish message.\n");
        }
        ztimer_sleep(ZTIMER_MSEC, PUBLICATION_INTERVAL * MS_PER_SEC);
    }
    return NULL;
}

int main(void) {
    if (connect_to_gateway() != EMCUTE_OK) {
        return 1;
    }

    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: Could not register topic.\n");
        return 1;
    }

    char pub_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(pub_stack, sizeof(pub_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}