#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>
#include <xtimer.h>

// MQTT broker IP and port
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

// Define the topic and message
#define TOPIC "esp32/state"
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"

static sock_udp_ep_t broker;
static mqttsn_ep_t gw;
static mqttsn_sock_t sock;
static mqttsn_t mqttsn;
static mqttsn_cbuf_t cbuf;

void setup_broker(sock_udp_ep_t *broker) {
    broker->family = AF_INET;
    broker->port = BROKER_PORT;
    ipv4_addr_from_str((ipv4_addr_t *)&broker->addr.ipv4, BROKER_IP);
}

void publish_state(const char *state) {
    size_t len = strlen(state);
    mqttsn_send_publish(&mqttsn, &sock, &gw, false, QOS_0, TOPIC, len, (const uint8_t *)state, false);
}

int main(void) {
    setup_broker(&broker);
    mqttsn_init(&mqttsn, &cbuf);
    mqttsn_cbuf_register(&mqttsn, &cbuf, &sock);

    printf("MQTT-SN example
");

    while (1) {
        publish_state(MESSAGE_WORK);
        xtimer_sleep(5);
        publish_state(MESSAGE_NOT_WORK);
        xtimer_sleep(5);
    }
    return 0;
}