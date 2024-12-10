
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

int main(void)
{
    sock_tcp_t sock;
    struct sockaddr_in server_addr;
    char buf[64];

    // Initialize the socket
    sock_tcp_create(&sock, NULL, NULL, SOCK_ADDR_ANY_NETIF, SOCK_PROTO_TCP);

    // Set up the server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, (struct sockaddr *)&server_addr, 0) < 0) {
        puts("Connection failed");
        return 1;
    }

    // MQTT CONNECT message
    const char connect_msg[] = "\x10\x0e\x00\x04MQTT\x04\x02\x00\x3c\x00\x00";
    sock_tcp_write(&sock, connect_msg, sizeof(connect_msg) - 1);

    // Wait for CONNACK
    sock_tcp_read(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT);

    while (1) {
        // MQTT PUBLISH message
        const char publish_msg[] = "\x30\x0a\x00\x05statework";
        sock_tcp_write(&sock, publish_msg, sizeof(publish_msg) - 1);

        xtimer_usleep(INTERVAL);
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
