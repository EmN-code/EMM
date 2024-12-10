#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "timex.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "mutex.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define BROKER_PORT         (1883U)
#define BROKER_ADDRESS      "47.102.103.1"
#define PUBLISH_INTERVAL    (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;

    printf("Published: %s\n", (char *)data);
}


int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    /* initialize the emcute thread */
    memset(stack, 0, sizeof(stack));
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO, "emcute");

    /* connect to gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%i\n", BROKER_ADDRESS, (int)gw.port);
        return 1;
    }
    printf("Connected to broker at %s:%d\n", BROKER_ADDRESS, BROKER_PORT);

    /* Publishing loop */
    while (1) {
        emcute_topic_t t;
        t.name = "state";

        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("error: unable to obtain topic ID");
            break;
        }

        const char *msg = "work";
        if (emcute_pub(&t, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    /* disconnect on exit */
    emcute_discon();

    return 0;
}