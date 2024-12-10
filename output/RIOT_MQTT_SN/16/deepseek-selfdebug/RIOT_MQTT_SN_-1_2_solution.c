
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define STATE_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    char msg[20];
    strncpy(msg, STATE_MESSAGE, sizeof(msg));
    msg[sizeof(msg) - 1] = '\0';

    if (emcute_pub(&topic, msg, strlen(msg), 0) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void) {
    sock_udp_t sock;
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);

    // Initialize the network stack
    if (netif_default == NULL) {
        puts("Error: no default network interface");
        return 1;
    }

    // Set the gateway address
    gw.addr.ipv4 = gw_addr;

    // Initialize the EMCUTE stack
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    // Connect to the MQTT-SN gateway
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN gateway");
        return 1;
    }
    puts("Connected to MQTT-SN gateway");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    printf("Registered topic: %s\n", topic.name);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
