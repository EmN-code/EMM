#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#include "emcute.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define MQTT_TOPIC "esp32/status"

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, "esp32_client");
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    inet_pton(AF_INET, BROKER_IP, &gw.addr);

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    char* state = "work";
    emcute_topic_t t;
    t.name = MQTT_TOPIC;
    emcute_reg(&t);
    
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        emcute_pub(&t, state, strlen(state), 0);
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}