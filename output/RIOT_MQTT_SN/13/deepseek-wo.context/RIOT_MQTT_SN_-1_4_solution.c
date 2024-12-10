
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

void publish_state(void) {
    emcute_topic_t topic;
    char message[12];

    strcpy(message, STATE_MESSAGE);

    if (emcute_reg(&topic, TOPIC_NAME) == EMCUTE_OK) {
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    if (emcute_con(&remote, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
