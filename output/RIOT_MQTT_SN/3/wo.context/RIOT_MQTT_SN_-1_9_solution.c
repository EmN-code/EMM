#include "ztimer.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID       "mqtt_sn"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT     (1883U)

#define MQTT_BROKER_IP  "47.102.103.1"
#define TOPIC_NAME      "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

static void on_mqtt_sn_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received: %.*s\n", (int)len, (char *)data);
}

static void *publisher_thread(void *arg) {
    (void)arg;
    char message[] = "work or not work";
    while (1) {
        int res = emcute_pub(&topic, message, sizeof(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error while publishing data: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    /* Initialize emcute */
    puts("--- Starting MQTT-SN example ---");

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    emcute_init(queue, sizeof(queue) / sizeof(msg_t), stack, sizeof(stack), EMCUTE_PRIO);

    /* Connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, MQTT_BROKER_IP);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to gateway; aborting\n");
        return 1;
    }

    /* Register topic */
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return 1;
    }

    /* Start publisher thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publisher_thread, NULL, "publisher_thread");

    return 0;
}