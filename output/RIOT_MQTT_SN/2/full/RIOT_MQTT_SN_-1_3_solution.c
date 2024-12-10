#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include <string.h>
#include "mutex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_LARGE];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    /* For now, this is just a stub function */
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, "esp32_node");

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "2001:db8:0:1::1");

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        ztimer_sleep(ZTIMER_USEC, 5000000);
    }

    emcute_topic_t t;
    memset(&t, 0, sizeof(t));
    strcpy(topics[0], "esp32/work_state");
    t.name = topics[0];

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return 1;
    }

    char *state = "work";
    while (1) {
        if (emcute_pub(&t, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}