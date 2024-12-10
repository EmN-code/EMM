#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/mqttsn.h"
#include "net/sock.h"

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_device"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define TOPIC "device/state"

static char mqtt_buf[128];
static char topic_name[] = TOPIC;

static int publish_state(sock_udp_ep_t *remote)
{
    // Prepare a message
    const char *state = "work";  // Change this to "not work" as needed
    size_t message_length = snprintf(mqtt_buf, sizeof(mqtt_buf), "%s", state);

    // Initialize the MQTT-SN PDU
    mqttsn_pdu_t pdu;
    if (mqttsn_pdu_init(&pdu, mqtt_buf, sizeof(mqtt_buf)) != 0) {
        puts("Error initializing PDU");
        return -1;
    }

    // Set the topic name
    if (mqttsn_pdu_set_publish(&pdu, topic_name, (void *)state, message_length, 0, 0) != 0) {
        puts("Error setting PUBLISH PDU");
        return -1;
    }

    // Send the PDU
    if (sock_udp_send(NULL, mqtt_buf, pdu.size, remote) < 0) {
        puts("Error sending PDU");
        return -1;
    }

    return 0;
}

int main(void)
{
    sock_udp_ep_t broker = {};
    broker.family = AF_INET;
    broker.port = BROKER_PORT;

    if (inet_pton(AF_INET, BROKER_ADDRESS, &broker.addr.ipv4) != 1) {
        puts("Error setting broker address");
        return 1;
    }

    while (1) {
        publish_state(&broker);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}