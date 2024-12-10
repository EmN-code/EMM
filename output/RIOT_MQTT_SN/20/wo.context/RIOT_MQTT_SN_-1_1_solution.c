#include <string.h>
#include <stdio.h>
#include "thread.h"
#include "net/emcute.h"
#include "xtimer.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           "esp32_client"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP           "47.102.103.1"
#define TOPIC_NAME          "device/state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;
static xtimer_ticks32_t interval = { .ticks32 = 5 * US_PER_SEC }; // 5 seconds interval

void *mqtt_sn_publisher(void *arg) {
    (void)arg;

    while (1) {
        /* publish "work" or "not work" randomly for demonstration */
        const char *state = rand() % 2 == 0 ? "work" : "not work";
        printf("Publishing state: %s\n", state);

        emcute_publish(&topic, state, strlen(state), EMCUTE_QOS_1);

        /* wait 5 seconds */
        xtimer_sleep(5);
    }

    return NULL;
}

static void on_con(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_sn_publisher, NULL, "mqtt_sn_publisher");
}

int main(void) {
    /* initialize emcute */
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    /* prepare the topic we will publish to */
    topic.name = TOPIC_NAME;
    topic.id = 0; /* let the broker assign the ID */

    /* connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, on_con, NULL);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return 1;
    }

    /* subscribe to topic so we can publish to it later */
    res = emcute_sub(&topic, EMCUTE_QOS_1);
    if (res != EMCUTE_OK) {
        printf("Failed to subscribe to topic: %d\n", res);
        return 1;
    }

    /* connect to MQTT-SN broker */
    on_con();
    
    return 0;
}