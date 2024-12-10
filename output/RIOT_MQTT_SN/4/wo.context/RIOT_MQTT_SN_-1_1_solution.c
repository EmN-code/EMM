#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "thread.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           "device"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)
#define MQTT_TOPIC          "status"
#define MQTT_MSG_WORK       "work"
#define MQTT_MSG_NOT_WORK   "not work"

static char stack[EMCUTE_STACK_SIZE];
static msg_t queue[8];
static emcute_sub_t subscriptions[1];

static char* topics[] = {MQTT_TOPIC};

void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int mqttsn_publish(const char *message) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s'\n", t.name);
        return 1;
    }

    printf("Published %s on topic %s\n", message, t.name);
    return 0;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, "47.102.103.1") == NULL) {
        puts("error: unable to parse IPv6 address\n");
        return 1;
    }

    if (emcute_con(&addr, "dflt") != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker\n");
        return 1;
    }
    
    while (1) {
        mqttsn_publish(MQTT_MSG_WORK);
        xtimer_sleep(5);
    }

    emcute_discon();

    return 0;
}