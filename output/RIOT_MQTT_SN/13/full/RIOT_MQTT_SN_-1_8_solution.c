#include "mma8x5x.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "ztimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "stdbool.h"
#include "stdlib.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdio.h"
#include "mbox.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "dht.h"
#include "stddef.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "unistd.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "timex.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"

#define EMCUTE_ID         "riot"
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME       "esp32/status"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

void *emcute_thread(void *varg) {
    (void)varg;
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_sleep(1000);
    printf("emcute thread started\n");

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("error: unable to parse remote address\n");
        return NULL;
    }

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        thread_sleep(1000);
    }

    puts("successfully connected\n");

    topic.name = TOPIC_NAME;
    while (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        thread_sleep(1000);
    }

    puts("successfully registered topic\n");

    char *status_msg = "work";
    while (1) {
        if (emcute_pub(&topic, status_msg, strlen(status_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("failed to publish data\n");
        } else {
            puts("data published\n");
        }
        thread_sleep(5000);
    }

    return NULL;
}

int main(void) {
    printf("Starting MQTT-SN Client\n");

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    return 0;
}