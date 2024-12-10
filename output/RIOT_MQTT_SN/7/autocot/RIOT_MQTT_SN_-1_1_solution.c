#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include "net/emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * 1000000U)
#define TOPIC_NAME "sensor/state"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACK_SIZE];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    emcute_topic_t t;
    t.name = TOPIC_NAME;
    t.id = 0;

    // Initialize EMCUTE client
    emcute_run(BROKER_PORT, queue, 8, EMCUTE_PRIO);

    // Connect to the broker
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_IP);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[ERROR] Unable to connect to [%s]:%d \n", BROKER_IP, BROKER_PORT);
        return NULL;
    }
    printf("[INFO] Connected to the broker\n");

    // Publish loop
    while(1) {
        const char *state_msg = "work";  // Modify this logic if needed
        emcute_publish(&t, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        printf("[INFO] Published state: %s\n", state_msg);
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    puts("MQTT-SN example program\n");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  mqtt_thread, NULL, "mqtt_thread");
    return 0;
}