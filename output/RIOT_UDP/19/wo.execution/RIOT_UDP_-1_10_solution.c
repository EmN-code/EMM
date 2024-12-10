#include "xtimer.h"
#include "net/sock/udp.h"
#include "string.h"
#include "arpa/inet.h" // inet_pton
#include "stdio.h" // printf

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MESSAGE "work"
#define DELAY 5

/* Thread stack for the main application */
char stack[THREAD_STACKSIZE_MAIN];

void *udp_thread(void *arg) {
    (void)arg;

    uint8_t buf[128];
    sock_udp_ep_t server = { .port = SERVER_PORT };

    /* Initialize the server structure with the IP address and port */
    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) != 1) {
        puts("Error: Could not convert IP address");
        return NULL;
    }

    sock_udp_t sock;
    /* Create the UDP socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: Could not create UDP socket");
        return NULL;
    }

    while (1) {
        /* Send the message to the server */
        ssize_t res = sock_udp_send(&sock, MESSAGE, strlen(MESSAGE), &server);
        if (res < 0) {
            printf("Error: Failed to send message: %ld\n", (long)res);
        } else {
            printf("Sent %ld bytes to %s:%d\n", (long)res, SERVER_IP, SERVER_PORT);
        }

        /* Sleep for the defined delay */
        xtimer_sleep(DELAY);
    }

    /* Close the UDP socket */
    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    /* Start the UDP thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, udp_thread, NULL, "udp_thread");

    return 0;
}