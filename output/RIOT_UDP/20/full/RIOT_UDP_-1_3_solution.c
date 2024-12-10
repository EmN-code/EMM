#include <stdio.h>  
#include <arpa/inet.h>  // Necessary for inet_pton function 
#include <xtimer.h>  // Required for xtimer_sleep
#include <net/sock/udp.h>  // Provides UDP socket APIs

#define SERVER_IP   "47.102.103.1"  // Server IP address
#define SERVER_PORT 20001  // Server port

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;
    int rc;
    char message_working[] = "work";
    char message_not_working[] = "not work";
    
    // Convert IPv4 address to binary and store in server struct
    if (inet_pton(AF_INET6, SERVER_IP, &server.addr.ipv6) <= 0) {
        perror("inet_pton failed");
        return 1;
    }

    // Creating the UDP socket
    rc = sock_udp_create(&sock, NULL, &server, 0);
    while (rc != 0) {
        printf("Failed to create UDP socket, retrying in 1 second...\n");
        xtimer_sleep(1);  // wait for 1 second before retrying
        rc = sock_udp_create(&sock, NULL, &server, 0);
    }

    printf("Successfully created UDP socket.\n");

    while (1) {
        rc = sock_udp_send(&sock, message_working, sizeof(message_working) - 1, &server);
        if (rc < 0) {
            printf("Error sending message: work\n");
            rc = sock_udp_send(&sock, message_not_working, sizeof(message_not_working) - 1, &server);
            if (rc < 0) {
                perror("Failed to send state message");
            } else {
                printf("Sent state: not working\n");
            }
        } else {
            printf("Sent state: working\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}