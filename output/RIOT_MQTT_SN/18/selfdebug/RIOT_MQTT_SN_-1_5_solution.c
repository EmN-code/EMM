#include <stdio.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("esp32_client")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

#define MQTT_BROKER_IP  "47.102.103.1"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

/* callback for receiving messages, defined but not used in main */
static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Received publication
");
}

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL; /* should never be reached */
}

int main(void) {
    /* setup networking */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    /* start emcute thread */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /*mqtt-sn configuration and connection*/
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    /* Publishing loop */
    while (1) {
        emcute_topic_t topic;
        topic.name = "state";

        /* Register topic */
        emcute_reg(&topic);

        /* Publish data */
        const char *message = "work";
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}