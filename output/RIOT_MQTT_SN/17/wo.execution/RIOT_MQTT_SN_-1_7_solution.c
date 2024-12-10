#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <msg.h>
#include <timex.h>
#include <unistd.h>  // For usleep function
#include <net/emcute.h>

#define EMCUTE_PORT       (1883U)
#define EMCUTE_ID         "mqtt_client"
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define DEFAULT_PUB_TOPIC "state"
#define DEFAULT_MSG "work"

static msg_t queue[8];
static emcute_topic_t topic;

void on_msg(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Received: %.*s\n", (int)len, (char *)data);
}

int main(void)
{
    /* initialize the message queue */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* setup UDP endpoint */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* start emcute thread */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    /* connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    /* setup topic for publishing */
    strcpy(topic.name, DEFAULT_PUB_TOPIC);
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    /* publish every 5 seconds */
    while (1) {
        if (emcute_pub(&topic, DEFAULT_MSG, strlen(DEFAULT_MSG), 0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        usleep(5 * 1000000U);  // sleep for 5 seconds
    }

    return 0;
}