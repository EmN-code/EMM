#include <ztimer.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U)
#define EMCUTE_ID "esp32_client"
#define EMCUTE_PORT (1883U)

static emcute_topic_t my_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);
    
    emcute_con(&gw, true, NULL, NULL, 0, 0);

    while (1) {
        const char *message = "work";
        emcute_pub(&my_topic, message, strlen(message), EMCUTE_QOS_0);

        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    strncpy(my_topic.name, "state", sizeof(my_topic.name));

    printf("Started MQTT-SN client with client id: %s\n", EMCUTE_ID);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * 1000);
    }
    return 0;
}