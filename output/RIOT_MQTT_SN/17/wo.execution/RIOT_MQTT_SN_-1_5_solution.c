#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <stdatomic.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <esp_system.h>
#include <mutex.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("### received publication: %.*s\n", (int)len, (char *)data);
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Convert IP address */
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) == 0) {
        puts("Error: unable to parse IPv4 address");
        return 1;
    }

    char *will_topic = "test/topic";
    char *will_msg = "disconnected";

    printf("[MQTT-SN] Trying to connect to gateway %s port %u\n",
           "47.102.103.1", gw.port);

    if (emcute_con(&gw, true, will_topic, will_msg, strlen(will_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = "work/state";
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return 1;
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published %s to %s\n", message, t.name);
        }

        xtimer_sleep(5);
    }

    return 0;
}