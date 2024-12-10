#include "xtimer.h"
#include "thread.h"
#include "sock/udp.h"
#include "net/ipv4/addr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (20001)
#define PERIOD (5U * US_PER_SEC)

static kernel_pid_t network_thread_pid;
static char network_thread_stack[STACK_SIZE];

static void *network_thread(void *arg) {
    (void)arg; // We don't use this arguments for now

    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    char *message = "Device is working";

    while (1) {
        sock_udp_send(&sock, message, strlen(message), &server);
        printf("Sent state to server: %s\n", message);
        xtimer_sleep(PERIOD / US_PER_SEC);
    }

    return NULL; // Never reaches here
}

int main(void) {
    network_thread_pid = thread_create(network_thread_stack, sizeof(network_thread_stack),
                                       THREAD_PRIORITY_MAIN - 1, 0,
                                       network_thread, NULL, "network thread");
    printf("Network thread PID: %d\n", network_thread_pid);
    return 0;
}