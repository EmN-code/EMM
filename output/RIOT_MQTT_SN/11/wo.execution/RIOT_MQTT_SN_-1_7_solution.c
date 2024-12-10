#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdint.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <emcute.h>

#define EMCUTE_ID           "RIOT_demo"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP           "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char topic_name[] = "status";

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    strncpy(topic.name, topic_name, sizeof(topic_name));
    emcute_reg(&topic);

    while (1) {
        const char *state_msg = "work";  // The message to publish
        emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        xtimer_sleep(5);
        state_msg = "not work";
        emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("Starting RIOT-CoAP Demo Application\n");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    return 0;
}