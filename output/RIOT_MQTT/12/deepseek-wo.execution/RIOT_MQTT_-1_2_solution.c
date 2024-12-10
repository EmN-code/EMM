
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <net/mqttsn.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static sock_tcp_ep_t broker_ep = SOCK_IPV4_EP_ANY;

static void connect_to_broker(void) {
    broker_ep.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    int res = sock_tcp_connect(&sock, &broker_ep, 0);
    if (res < 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
    printf("Connected to MQTT broker\n");
}

static void publish_state(void) {
    uint8_t buf[128];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, TOPIC);
    memcpy(pdu.payload, PAYLOAD, PAYLOAD_LEN);
    pdu.payload_len = PAYLOAD_LEN;

    ssize_t len = sock_tcp_send(&sock, buf, pdu.payload_len);
    if (len < 0) {
        printf("Failed to send state\n");
    } else {
        printf("State published\n");
    }
}

int main(void) {
    connect_to_broker();

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        publish_state();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
