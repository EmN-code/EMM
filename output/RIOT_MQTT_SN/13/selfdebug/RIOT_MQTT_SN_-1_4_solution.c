#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "emcute.h"

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)
#define MESSAGE_INTERVAL (5U * US_PER_SEC) // Send a message every 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char *status_message = "work or not work";
static sock_udp_ep_t gateway = { .family = AF_INET, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Do nothing as this example doesn't handle incoming messages
}

static void init_communication(void) {
    msg_init_queue(queue, 8);

    gateway.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &gateway.addr.ipv4);

    emcute_run(EMCUTE_PORT, "device001");
}

int main(void) {
    puts("MQTT-SN example connecting to broker at 47.102.103.1:1883");

    init_communication();

    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = "state/update";

    while (1) {
        if (emcute_pub(&topic, status_message, strlen(status_message), EMCUTE_QOS_0) == EMCUTE_OK) {
            printf("Published: %s\n", status_message);
        } else {
            printf("Publication failed\n");
        }

        xtimer_usleep(MESSAGE_INTERVAL);
    }
    return 0;
}