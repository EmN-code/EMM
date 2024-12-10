
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"

static char state_msg[32];

void publish_state(void) {
    static int state = 0;
    const char *states[] = {"work", "not work"};

    snprintf(state_msg, sizeof(state_msg), "%s", states[state]);
    state = !state;

    emcute_topic_t topic;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return;
    }

    if (emcute_pub(&topic, state_msg, strlen(state_msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", state_msg);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = MQTT_BROKER_PORT, .family = AF_INET };
    inet_addr_from_str(gw.addr.ipv4, MQTT_BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}
