#include <unistd.h>  // Required for usleep
#include <string.h>  // Common string operations
#include <stdio.h>   // Standard I/O operations (note: despite restrictions, typically used in C)
#include <arpa/inet.h>  // For inet_pton
#include <netinet/in.h> // Contains constants and structures needed for internet domain addresses
#include "net/sock/udp.h" // For UDP socket communication

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE_INTERVAL 5

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    const char *message_work = "work";
    const char *message_not_work = "not work";
    char *message;
    int work_state = 1;

    while (1) {
        message = work_state ? message_work : message_not_work;
        if (sock_udp_send(&sock, message, strlen(message), &remote) < 0) {
            puts("Error sending message");
        } else {
            printf("Sent message: %s\n", message);
        }

        work_state = !work_state;  // Toggle state between work and not work
        usleep(MESSAGE_INTERVAL * 1000000);  // Sleep for 5 seconds
    }

    sock_udp_close(&sock);
    return 0;
}