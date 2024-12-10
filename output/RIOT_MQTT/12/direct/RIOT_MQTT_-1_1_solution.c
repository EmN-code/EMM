#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/mqttsn_pkt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_mqtt_client"
#define TOPIC "device/state"

static void publish_state(const char* state) {
    printf("Publishing state: %s\n", state);

    sock_udp_ep_t broker_endpoint = {};
    broker_endpoint.family = AF_INET;
    broker_endpoint.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&broker_endpoint.addr.ipv6, BROKER_IP);
    broker_endpoint.port = BROKER_PORT;

    mqttsn_pkt_t pkt;
    mqttsn_pkt_init(&pkt, MQTT_SN_PUBLISH);
    mqttsn_pkt_set_flags(&pkt, QOS_0, false, false);
    mqttsn_pkt_set_topic(&pkt, TOPIC, strlen(TOPIC));
    mqttsn_pkt_pack_data(&pkt, (const uint8_t *)state, strlen(state));

    if (mqttsn_sock_send(&pkt, &broker_endpoint) < 0) {
        printf("Error: unable to send MQTT-SN packet\n");
    }
}

int main(void) {
    // Initialize connection
    printf("MQTT-SN client initializing\n");

    // Main Loop for publishing state every 5 seconds
    while (1) {
        // Simulate fetching the state of the device
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        publish_state(state);

        // Sleep for 5 seconds
        sleep(5);
    }

    return 0;
}