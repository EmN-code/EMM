#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "inet.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define EMCUTE_ID "gertrud"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT (1883U)
#define TOPIC_MAXLEN (64U)

static char stack[STACKSIZE];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, STACKSIZE, EMCUTE_ID);
    return NULL; // This line should never be reached
}

int main(void) {
    puts("RIOT MQTT-SN example application using EMCUTE");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Continuously try to connect to the gateway
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Successfully connected to gateway at %s\n", "47.102.103.1");
            break;
        }
        else {
            printf("Connection to gateway failed, retrying...\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    memset(&t, 0, sizeof(t));
    strcpy(t.name, "state");

    while (emcute_reg(&t) != EMCUTE_OK) {
        printf("Registration failed, retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&t, message, strlen(message), flags) == EMCUTE_OK) {
            printf("Published message: %s\n", message);
        }
        else {
            printf("Failed to publish message\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}