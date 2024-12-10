
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 1024
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static char mqtt_connect_msg[] = "\x10\x0e\x00\x04MQTT\x04\x02\x00\x3c\x00\x0aRIOT_MQTT";
static char mqtt_publish_msg[] = "\x30\x11\x00\x05statework or not work";

static void send_mqtt_message(sock_tcp_t *sock, const char *msg, size_t len) {
    if (sock_tcp_write(sock, msg, len) < 0) {
        puts("Error sending MQTT message");
    }
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    char buf[BUF_SIZE];

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    // Create TCP socket
    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return 1;
    }

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &remote, SOCK_NO_TIMEOUT) < 0) {
        puts("Error connecting to MQTT broker");
        sock_tcp_close(&sock);
        return 1;
    }

    // Send MQTT CONNECT message
    send_mqtt_message(&sock, mqtt_connect_msg, sizeof(mqtt_connect_msg) - 1);

    // Main loop to publish state every 5 seconds
    while (1) {
        // Send MQTT PUBLISH message
        send_mqtt_message(&sock, mqtt_publish_msg, sizeof(mqtt_publish_msg) - 1);

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
