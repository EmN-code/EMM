#include <ztimer.h>
#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <thread.h>
#include <net/emcute.h>

#define EMCUTE_PORT         (1883U)
#define MQTT_TOPIC_NAME     "esp32/status"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

void *emcute_thread_handler(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "emcute");
    return NULL;
}

int main(void) {
    /* set the message queue */
    msg_init_queue(queue, 8);

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread_handler, NULL, "emcute");

    /* setup the ip address and port of the broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* connect to mqtt-sn gateway */
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    /* register topic */
    topic.name = MQTT_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        /* publish context */
        const char *message = (rand() % 2 == 0) ? "work" : "not work";

        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Published: %s\n", message);
        }

        /* wait 5 seconds */
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}
