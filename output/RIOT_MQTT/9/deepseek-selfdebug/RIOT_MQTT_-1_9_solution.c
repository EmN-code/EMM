
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    char buf[128];
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create a TCP socket
    res = sock_tcp_create(&sock, NULL, &remote, 0);
    if (res < 0) {
        printf("Error creating socket: %d\n", res);
        return 1;
    }

    // Connect to the MQTT broker
    res = sock_tcp_connect(&sock, &remote, 0);
    if (res < 0) {
        printf("Error connecting to server: %d\n", res);
        sock_tcp_close(&sock);
        return 1;
    }

    // MQTT Connect Packet
    const char connect_packet[] = "\x10\x0e\x00\x04MQTT\x04\x02\x00\x3c\x00\x00";
    sock_tcp_send(&sock, connect_packet, sizeof(connect_packet) - 1, 0);

    while (1) {
        // MQTT Publish Packet
        const char publish_packet[] = "\x30\x0b\x00\x05statework";
        sock_tcp_send(&sock, publish_packet, sizeof(publish_packet) - 1, 0);

        printf("Published state: work\n");

        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
