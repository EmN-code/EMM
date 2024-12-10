#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "net/sock/udp.h"

// Function to send a UDP message
void send_udp_message(sock_udp_ep_t *remote, const char *message) {
    sock_udp_t sock;
    ssize_t res = sock_udp_create(&sock, NULL, remote, 0);
    if (res < 0) {
        printf("Error creating UDP socket\n");
        return;
    }

    size_t len = strlen(message);
    res = sock_udp_send(&sock, message, len, remote);
    if (res < 0) {
        printf("Error sending UDP message\n");
    }

    sock_udp_close(&sock);
}

// Main function
int main(void) {
    char *message1 = "work";
    char *message2 = "not work";
    sock_udp_ep_t remote = { .family = AF_INET, .port = 20001 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    while (1) {
        send_udp_message(&remote, message1);
        sleep(5);

        send_udp_message(&remote, message2);
        sleep(5);
    }

    return 0;
}