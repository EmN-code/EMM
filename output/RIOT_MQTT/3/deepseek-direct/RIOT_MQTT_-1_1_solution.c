
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/conn/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLIENT_ID "esp32_client"

#define BUFFER_SIZE 128

static sock_tcp_t sock;
static char buffer[BUFFER_SIZE];

void mqtt_connect(void) {
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    ipv4_addr_t broker_addr;
    ipv4_addr_from_str(&broker_addr, MQTT_BROKER_IP);
    remote.addr.ipv4 = broker_addr;

    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    // MQTT Connect Packet
    uint8_t connect_packet[] = {
        0x10, 0x16, 0x00, 0x04, 'M', 'Q', 'T', 'T', 0x04, 0x02, 0x00, 0x3C, 0x00, 0x0B, 'e', 's', 'p', '3', '2', '_', 'c', 'l', 'i', 'e', 'n', 't'
    };

    sock_tcp_send(&sock, connect_packet, sizeof(connect_packet), 0);
}

void mqtt_publish(const char *topic, const char *payload) {
    uint8_t publish_packet[BUFFER_SIZE];
    uint8_t *ptr = publish_packet;

    *ptr++ = 0x30; // MQTT PUBLISH packet
    *ptr++ = 0x00; // Remaining length (to be filled later)

    // Topic length
    uint16_t topic_len = strlen(topic);
    *ptr++ = (topic_len >> 8) & 0xFF;
    *ptr++ = topic_len & 0xFF;

    // Topic
    memcpy(ptr, topic, topic_len);
    ptr += topic_len;

    // Payload
    uint16_t payload_len = strlen(payload);
    memcpy(ptr, payload, payload_len);
    ptr += payload_len;

    // Fill in the remaining length
    publish_packet[1] = ptr - publish_packet - 2;

    sock_tcp_send(&sock, publish_packet, ptr - publish_packet, 0);
}

int main(void) {
    puts("Starting MQTT client...");

    // Initialize network stack
    if (net_init() < 0) {
        puts("Error: could not initialize network stack");
        return 1;
    }

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish(MQTT_TOPIC, MQTT_PAYLOAD);
        xtimer_sleep(5);
    }

    return 0;
}
