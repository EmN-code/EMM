
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL 5  // in seconds

int main(void)
{
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    char buffer[128];
    int res;

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    // Create a UDP socket
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    while (1) {
        // Prepare the payload
        memcpy(buffer, PAYLOAD, PAYLOAD_LEN);

        // Send the payload to the MQTT broker
        res = sock_udp_send(&sock, buffer, PAYLOAD_LEN, &remote);
        if (res < 0) {
            puts("Error sending data");
        } else {
            printf("Sent %d bytes to %s:%d\n", res, EMQX_SERVER_IP, EMQX_SERVER_PORT);
        }

        // Wait for 5 seconds before sending the next message
        sleep(INTERVAL);
    }

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}
