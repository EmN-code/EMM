#include "thread.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"  
#include "net/emcute.h"
#include "mutex.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"

#define MQTT_TOPIC_MAXLEN 64
#define MQTT_MESSAGE_MAXLEN 64
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static char status_msg[MQTT_MESSAGE_MAXLEN];
static emcute_topic_t mqtt_topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

void publish_status(void) {
    int res = emcute_pub(&mqtt_topic, status_msg, strlen(status_msg), 0);
    if (res != EMCUTE_OK) {
        puts("Error: Unable to publish data");
    }
}

void run(void) {
    while (1) {
        strcpy(status_msg, "work");  // or "not work" based on actual status
        publish_status();
        xtimer_sleep(5);
    }
}

int main(void) {
    /* Initialize and connect to the MQTT-SN broker */
    strcpy(mqtt_topic.name, "device/status");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Unable to connect to broker");
        return 1;
    }

    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        puts("Error: Unable to register topic");
        return 1;
    }

    puts("MQTT-SN client is running");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, run, NULL, "run");
    return 0;
}