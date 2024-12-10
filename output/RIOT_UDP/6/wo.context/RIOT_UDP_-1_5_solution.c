#include <net/gnrc/ipv6.h>
#include <net/gnrc.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <stdio.h>

/* Define the server address and port */
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001

/* Timer interval set to 5 seconds (5 * 10^6 microseconds) */
#define TIMER_INTERVAL (5U * 1000000U)

void *udp_sender_thread(void *arg) {
    (void)arg; // Unused
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);

    char *message = "Device State: Work"; // Message to send

    while (1) {
        ssize_t res = sock_udp_send(NULL, message, strlen(message), &remote);
        if (res < 0) {
            printf("Error sending message: %d\n", (int)res);
        } else {
            printf("Message sent successfully!\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    puts("UDP device state sender started");

    /* Start the UDP sender thread */
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  udp_sender_thread, NULL, "udp_sender_thread");

    /* Keep the main alive */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}