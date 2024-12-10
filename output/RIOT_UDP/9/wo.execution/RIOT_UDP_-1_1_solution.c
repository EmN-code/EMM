#include <thread.h>
#include <sched.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define INTERVAL_SEC 5

static void *udp_client(void *arg) {
    (void)arg; // We don't use this in the thread function
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    // Attempt to create server endpoint
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR) == NULL) {
        printf("Error converting server address\n");
        return NULL;
    }

    sock_udp_t sock;
    // Create UDP socket
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    char *state_msg;  
    while (1) {
        // Here, we determine the device state somehow
        state_msg = "Device is working";  // For the sake of example

        // Send UDP packet
        ssize_t res = sock_udp_send(&sock, state_msg, strlen(state_msg), &server);
        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent message: %s\n", state_msg);
        }

        // Wait for specified interval
        xtimer_sleep(INTERVAL_SEC);
    }
    return NULL;
}

int main(void) {
    // The stack size for the thread
    char stack[THREAD_STACKSIZE_DEFAULT];
    // Thread attributes
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, udp_client, NULL, "udp_thread");
    return 0;
}