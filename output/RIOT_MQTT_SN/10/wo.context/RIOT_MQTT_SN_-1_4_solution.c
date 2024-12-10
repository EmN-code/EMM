#include <net/sock/udp.h>
#include <net/sock.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "msg.h"
#include "emcute.h"

#define EMCUTE_PORT     1883
#define SERVER_ADDR     "47.102.103.1"
#define CLIENT_ID       "esp32_client"
#define TOPIC_NAME      "state"
#define MESSAGE         "work"
#define SLEEP_DURATION  5

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
    printf("Publication received
");
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET6 };  // Use appropriate AF_INET for IPv6 or IPv4
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr.ipv6);  // Assuming an IPv6 address structure
    gw.port = EMCUTE_PORT;

    emcute_run(EMCUTE_PORT, CLIENT_ID);

    /* Register the topic */
    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    while (1) {
        /* Publish the message */
        emcute_pub(&topic, MESSAGE, strlen(MESSAGE), EMCUTE_QOS_0);

        /* Sleep for specified duration */
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}