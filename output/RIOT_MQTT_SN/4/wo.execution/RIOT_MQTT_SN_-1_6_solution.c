#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "string.h"
#include "msg.h"
#include "mutex.h"
#include "timex.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "net/emcute.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    emcute_run(EMCUTE_PORT, "emcute");
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("### got publication for topic "%s" [%i] ###\n",
           topic->name, (int)topic->id);
    printf("%s\n", in);
}

static int pub(char *topic_name, char *data) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[FAILED] unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("[FAILED] unable to publish data\n");
        return 1;
    }

    printf("[OK] Published %s on topic %s\n", data, t.name);
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 }; 
    while (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {}
    
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n",
               "47.102.103.1", (int)gw.port);
        return 1;
    }
    printf("[OK] Connected to gateway at [%s]:%d\n",
           "47.102.103.1", gw.port);

    while(1) {
        pub("device/state", "working");
        xtimer_sleep(5);
    }

    return 0;
}