#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "stdatomic.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "unistd.h"
#include "stdio.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "stddef.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "dht.h"
#include "stdlib.h"
#include "arpa/inet.h"
#include "net/gcoap.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];

static int get_topic_id(emcute_topic_t *topic) {
    int res = emcute_reg(topic);
    while (res != EMCUTE_OK) {
        // Retry every second if registration fails
        xtimer_sleep(1);
        res = emcute_reg(topic);
    }
    return res;
}

static void *publisher(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    strcpy(topic.name, "device/state");
    get_topic_id(&topic);

    while (1) {
        const char *message = "Work
";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error publishing data\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    int res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    while (res != 1) {
        printf("Failed to parse address\n");
        xtimer_sleep(1);
        res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    }

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        xtimer_sleep(1);
    }
    return EMCUTE_OK;
}

int main(void) {
    puts("MQTT-SN client application``);
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher, NULL, "emcute_pub");
    connect_broker();

    return 0;
}
