#include <ztimer.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <mutex.h>
#include <evtimer_mbox.h>
#include <stdatomic.h>
#include <stdio.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <msg.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <kernel_defines.h>
#include <dht.h>
#include <stddef.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <unistd.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <timex.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>

#define EMCUTE_ID       "RIOTnode"
#define EMCUTE_PORT     1883
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP  "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[16];
static int num_subscriptions = 0;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    _long_alert();
    return NULL;
}

static int _long_alert(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return 1;
    }

    unsigned flags = EMCUTE_QOS_0;
    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection refused, retrying...\n");
        ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);
    }

    emcute_topic_t t;
    memset(&t, 0, sizeof(t));
    strcpy(t.name, "status");

    while (emcute_reg(&t) != EMCUTE_OK) {
        printf("Getting topic ID failed, retrying...\n");
        ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);
    }

    while (true) {
        if (emcute_pub(&t, "work", strlen("work"), flags) != EMCUTE_OK) {
            printf("Publishing failed, retrying...\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}

int main(void) {
    memset(subscriptions, 0, sizeof(subscriptions));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    return 0;
}