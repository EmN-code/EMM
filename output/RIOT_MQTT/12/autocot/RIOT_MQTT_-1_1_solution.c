#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/dbg.h>
#include <net/af.h>
#include <net/sock/tcp.h>
#include <net/sock_udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL 5

// MQTT packet control
#define MQTT_CONNECT 0x10
#define MQTT_PUBLISH 0x30
#define MQTT_DISCONNECT 0xE0

void mqtt_connect(sock_udp_t *sock, const sock_udp_ep_t *remote) {
    // Prepare MQTT CONNECT packet
    uint8_t packet[] = {
        MQTT_CONNECT,  // Control packet type
        12,  // Remaining length
        0x00, 0x04, 'M', 'Q', 'T', 'T',  // Protocol Name
        0x04,  // Protocol Level
        0x02,  // Connect Flag (Clean session)
        0x00, 0x3C,  // Keep Alive
        0x00, 0x00   // Payload (client identifier length zero)
    };
    
    sock_udp_send(sock, packet, sizeof(packet), remote);
}

void mqtt_publish(sock_udp_t *sock, const sock_udp_ep_t *remote, const char *payload) {
    // Prepare MQTT PUBLISH packet
    uint8_t packet[256];
    size_t payload_len = strlen(payload);
    size_t len = 0;
    
    packet[len++] = MQTT_PUBLISH;
    packet[len++] = payload_len + 2 + 2; // Remaining length
    
    // Topic ("status")
    packet[len++] = 0x00;
    packet[len++] = 0x06;
    memcpy(&packet[len], "status", 6);
    len += 6;
    
    // Payload
    memcpy(&packet[len], payload, payload_len);
    len += payload_len;
    
    sock_udp_send(sock, packet, len, remote);
}

void mqtt_disconnect(sock_udp_t *sock, const sock_udp_ep_t *remote) {
    // Prepare MQTT DISCONNECT packet
    uint8_t packet[] = {MQTT_DISCONNECT, 0};
    
    sock_udp_send(sock, packet, sizeof(packet), remote);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    mqtt_connect(&sock, &remote);

    while (1) {
        mqtt_publish(&sock, &remote, "work");
        sleep(PUBLISH_INTERVAL);
        mqtt_publish(&sock, &remote, "not work");
        sleep(PUBLISH_INTERVAL);
    }

    mqtt_disconnect(&sock, &remote);
    sock_udp_close(&sock);

    return 0;
}