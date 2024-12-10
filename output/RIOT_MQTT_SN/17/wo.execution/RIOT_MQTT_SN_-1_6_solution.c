#include "ztimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t subscriptions[NUMOFSUBS];
static emcute_topic_t topics[NUMOFSUBS];

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("### received publication for topic '%s' [%i]###\n",
           topic->name, (int)topic->id);
    printf("Data: %.*s\n", len, in);
}

static int setup_mqtt(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };    
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("error: unable to parse IPv4 address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    emcute_topic_t t;
    strcpy(t.name, "status");
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    printf("Successfully connected to gateway at [47.102.103.1]:%d\n", EMCUTE_PORT);
    return 0;
}

int main(void)
{
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    if (setup_mqtt() != 0) {
        return 1;
    }

    while (1) {
        const char *status_msg = "work";
        emcute_topic_t *t = &topics[0];

        if (emcute_pub(t, status_msg, strlen(status_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        } else {
            printf("Published `%s` to topic `%s`\n", status_msg, t->name);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
