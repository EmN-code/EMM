#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "kernel/thread.h"
#include "xtimer.h"

#define INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "status"
#define MQTT_DATA "work or not work"
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define EMCUTE_ID "esp32-client"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_queue_init(queue, ARRAY_SIZE(queue));

    emcute_run(MQTT_BROKER_PORT, EMCUTE_ID);

    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);
        emcute_pub_t pub;
        pub.topic.name = MQTT_TOPIC;
        pub.data = MQTT_DATA;
        pub.data_len = strlen(MQTT_DATA);
        pub.qos = EMCUTE_QOS_0;

        if (emcute_pub(&pub) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
    }

    return NULL;
}

int main(void) {
    uuid_t addr;
    ipv6_addr_from_str(&addr, MQTT_BROKER_IP);
    
    char *args[] = {EMCUTE_ID, MQTT_BROKER_IP};
    if (emcute_con(args, NULL, 2) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "MQTT-thread");

    return 0;
}