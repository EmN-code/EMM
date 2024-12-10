#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U * US_PER_SEC)

static void send_udp_message(sock_udp_ep_t *remote) {
    char *message = "Device state: working";
    sock_udp_t sock;
    ssize_t res;

    // Create a socket
    res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    // Send the message
    res = sock_udp_send(&sock, message, strlen(message), remote);
    if (res < 0) {
        printf("Error sending message\n");
    } else {
        printf("Message sent\n");
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF }; 
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        printf("Invalid server IP address\n");
        return 1;
    }  
    server.port = SERVER_PORT;

    while (1) {
        send_udp_message(&server);
        xtimer_sleep(5);
    }

    return 0;
}