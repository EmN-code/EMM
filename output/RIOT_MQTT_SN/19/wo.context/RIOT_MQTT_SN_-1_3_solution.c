#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (1U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic; /* unused */
    printf("Received publication with length: %u\n", (unsigned)len);
    if (data != NULL) {
        printf("Payload: %s\n", (char *)data);
    }
}

int main(void) {
    /* setup our subscription */
    memset(&topic, 0, sizeof(topic));
    topic.name = "test";

    /* setup UDP endpoint for mqtt-sn connection */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    /* connect to the server */
    if (emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_CS) != EMCUTE_OK) {
        puts("error: unable to connect to server");
        return 1;
    }

    /* subscribe */
    emcute_sub_t sub;
    sub.topic.name = topic.name;
    sub.cb = on_pub;
    if (emcute_sub(&sub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to subscribe");
        return 1;
    }
    printf("Now subscribed to %s\n", topic.name);

    /* publish messages every 5 seconds */
    while (1) {
        const char *messages[2] = {"work", "not work"};
        static uint8_t idx = 0;

        if (emcute_pub(&topic, messages[idx], strlen(messages[idx]), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data
");
        }
        idx = (idx + 1) % 2;
        ztimer_sleep(ZTIMER_MSEC_BASE, 5 * 1000);
    }

    /* disconnect */
    if (emcute_discon() != EMCUTE_OK) {
        puts("error: unable to disconnect");
        return 1;
    }

    return 0;
}