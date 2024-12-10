#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/conn.h"

#define EMCUTE_ID       "ESP32"
#define EMCUTE_PORT     1883
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define BROKER_ADDRESS  "47.102.103.1"
#define PUB_INTERVAL    (5U * US_PER_SEC)
#define TOPIC           "device/state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sock_t sock;

tstatic void *emcute_thread(void *arg) {
    (void)arg;
    char *topic = TOPIC;

    while (1) {
        const char *state = "work";
        if (rand() % 2 == 0) {
            state = "not work";
        }

        emcute_topic_t t;
        unsigned flags = EMCUTE_QOS_0;

        t.name = topic;
        if (emcute_publish(&t, state, strlen(state), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("published: %s to topic %s\n", state, topic);
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    /* initialize the system */
    srand(time(NULL));

    emcute_iface_t *iface = emcute_iface_init();
    if (emcute_sock_init(&sock, EMCUTE_PORT, EMCUTE_PRIO, iface) != EMCUTE_OK) {
        printf("error initializing emcute\n");
        return 1;
    }

    /* connect to MQTT broker */
    sock.remote.addr = ntp_pton4(BROKER_ADDRESS);
    sock.remote.port = EMCUTE_PORT;
    if (emcute_conn(&sock) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%hu\n", BROKER_ADDRESS, EMCUTE_PORT);
        return 1;
    }

    printf("connected to MQTT broker at %s\n", BROKER_ADDRESS);

    /* start publishing thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* idle loop */
    while (1) {
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}