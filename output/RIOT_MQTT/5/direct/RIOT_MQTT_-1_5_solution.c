#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "net/emcute.h"

#define EMCUTE_PORT          (1883U)
#define EMCUTE_PRIO          (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_ADDRESS  "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define PUB_TOPIC            "status"
#define INTERVAL_USEC        (5U * US_PER_SEC)  // Interval for 5 seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

void on_pub(const emcute_resend_handler_t sender, const char *topic, 
            const void *payload, size_t payload_len, void *user) {
    (void)sender;
    (void)topic;
    (void)payload;
    (void)payload_len;
    (void)user;
    puts("Successfully published");
}

/* Run the subscriber thread */
static void *emcute_thread(void *arg) {
    emcute_run(EMCUTE_PORT, EMCUTE_PRIO);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    emcute_sub_t sub;
    sub.cb = on_pub;
    sub.topic.name = PUB_TOPIC;
    emcute_sub(&sub, 1);

    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_ADDRESS);
    gw.port = MQTT_BROKER_PORT;

    unsigned flags = EMCUTE_QOS_0;

    while (1) {
        const char *message = "work";
        emcute_pub(&sub.topic, message, strlen(message), flags);
        xtimer_usleep(INTERVAL_USEC);
    }

    return 0;
}