#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mqttclient.h>
#include <dht_params.h>
#include <dht.h>
#include <esp_wifi.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <msg.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, "esp32");
    return NULL; // should never be reached
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    static sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = 1883
    };

    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    emcute_topic_t topic;
    unsigned flags = EMCUTE_QOS_0;

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    char *topic_name = "device/state";
    topic.name = topic_name;
    emcute_reg(&topic);

    while (1) {
        const char *state = "work"; // Assume the device is working
        emcute_pub(&topic, state, strlen(state), flags);
        ztimer_sleep(ZTIMER_MSEC, 5000); // 5 seconds
    }

    return 0;
}