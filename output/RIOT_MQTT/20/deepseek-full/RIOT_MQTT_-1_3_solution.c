
#include <net/sock.h>
#include <net/af.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <xtimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    // Create UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    char message[20];
    strcpy(message, MQTT_MESSAGE);

    while (1) {
        // Send message to MQTT broker
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error sending message");
        } else {
            printf("Message sent: %s\n", message);
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    // Close socket
    sock_udp_close(&sock);

    return 0;
}
