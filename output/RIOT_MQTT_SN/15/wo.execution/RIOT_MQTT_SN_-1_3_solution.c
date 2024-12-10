#include "thread.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "arpa/inet.h"

#define EMCUTE_ID       (1U)
#define EMCUTE_PORT     (1883U)
#define BROKER_IP       "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[EMCUTE_ID];

static void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;

    /* Initialize address */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    /* Setup connection */
    if (emcute_con(&gw, true, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return NULL;
    }

    /* Prepare topic */
    t.name = "state";
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    /* Publish loop */
    while (1) {
        emcute_pub(&t, "work", strlen("work"), EMCUTE_QOS_0);
        ztimer_sleep(ZTIMER_USEC, 5 * US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    /* Create thread for MQTT-SN operations */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");
    /* Running main will yield to other threads */
    thread_yield();
    return 0;
}