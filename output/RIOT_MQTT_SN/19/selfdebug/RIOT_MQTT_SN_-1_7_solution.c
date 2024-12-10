#include "net/sock/udp.h"
#include "net/protnum.h"
#include "net/mqttsn.h"
#include "net/ipv6/addr.h"
#include "emcute.h"
#include "xtimer.h"
#include "thread.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

// MQTT-SN topic
static const emcute_topic_t topic = {
    .name = "state",
    .id = 12345 // Assuming a pre-registered topic ID for "state"
};

// Message buffer for the thread
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher_thread(void *arg) {
    (void)arg;
    char *state_message = "work";

    while (1) {
        int res = emcute_pub(&topic, state_message, strlen(state_message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            puts("pub failed");
        }
        state_message = strcmp(state_message, "work") == 0 ? "not work" : "work";
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the emcute network stack
    emcute_sock_t sock;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("unable to connect to MQTT broker");
        return 1;
    }

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("unable to register topic");
        emcute_discon();
        return 1;
    }

    // Create publisher thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publisher_thread, NULL, "publisher");

    return 0;
}