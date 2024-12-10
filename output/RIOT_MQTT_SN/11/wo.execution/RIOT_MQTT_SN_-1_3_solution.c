#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "arpa/inet.h"

#define EMCUTE_ID       "mqtt_test"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT     (1883U)
#define EMCUTE_BUFSIZE  (128U)
#define TOPIC_NAME      "state"

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_BUFSIZE);
    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_DEFAULT];
    static emcute_topic_t topic;
    uint8_t flags = EMCUTE_QOS_0;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) != 1) {
        puts("error: unable to parse gateway address");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }
    
    while (1) {
        const char *state = "work or not work";
        if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
            puts("error: unable to publish");
        } else {
            puts("published successfully");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}