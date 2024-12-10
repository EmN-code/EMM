#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    20001
#define SEND_INTERVAL  5
#define MSG_WORK       "work"
#define MSG_NOT_WORK   "not work"

static sock_udp_ep_t remote;

int main(void)
{
    // Socket for UDP
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV6_EP_NONE;
    char message[20];
    int res;

    // Convert the string IP address to a binary one using inet_pton
    while (1) {
        res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
        if (res <= 0) {
            puts("inet_pton failed, retrying...");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Create UDP socket
    while (1) {
        res = sock_udp_create(&sock, &local, NULL, 0);
        if (res < 0) {
            puts("Error creating UDP socket, retrying...");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    // Main loop to send UDP messages every 5 seconds
    while (1) {
        strcpy(message, MSG_WORK);  // Example device state
        ssize_t len = sock_udp_send(&sock, message, strlen(message), &remote);
        if (len < 0) {
            puts("Error sending UDP message, retrying...");
            continue;
        }
        puts("Message sent!");

        // Wait for the specified interval
        xtimer_sleep(SEND_INTERVAL);
    }

    // Close the socket (not reached in this example)
    sock_udp_close(&sock);

    return 0;
}