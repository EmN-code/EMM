#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "emcute.h"

#define EMCUTE_PORT       (1883U)
#define MQTT_TOPIC_MAXLEN (64U)
#define BROKER_IP         "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, "riot_client");
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute thread");

    /* connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Unable to connect to [%s]:%d\n", BROKER_IP, EMCUTE_PORT);
        return 1;
    }

    emcute_topic_t topic;
    char topic_name[MQTT_TOPIC_MAXLEN] = "esp32/status";
    topic.name = topic_name;

    xtimer_ticks32_t last = xtimer_now();
    while (1) {
        /* publish state every 5 seconds */
        char message[50] = "ESP32 is working.";
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        xtimer_periodic_wakeup(&last, 5 * US_PER_SEC);
    }

    emcute_discon();
    return 0;
}