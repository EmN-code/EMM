#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_ID           "my_mqtt_sn_client"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP           "47.102.103.1"
#define TOPIC_NAME          "device/state"
#define PUB_INTERVAL_SEC    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t state_topic;

void *emcute_thread(void *arg);

int main(void) {
    char *pub_data = "work or not work";
    sock_udp_ep_t gw = { .family = AF_INET,
                         .netif = SOCK_ADDR_ANY_NETIF,
                         .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_IP) == NULL) {
        puts("Error: unable to parse broker IP");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute_thread");

    while(1) {
        xtimer_sleep(PUB_INTERVAL_SEC);
        if (emcute_pub(&state_topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        else {
            puts("Data published successfully!");
        }
    }

    return 0;
}

void *emcute_thread(void *arg) {
    sock_udp_ep_t gw = { .family = AF_INET,
                         .netif = SOCK_ADDR_ANY_NETIF,
                         .port = EMCUTE_PORT };

    emcute_con(&gw, true, NULL, NULL, 0, 0);
    
    state_topic.name = TOPIC_NAME;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    return NULL;
}