#include <ztimer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread.h>

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define STATE_UPDATE_INTERVAL (5U)  // 5 seconds

static kernel_pid_t udp_thread_pid;
static char udp_thread_stack[THREAD_STACKSIZE_DEFAULT];

static void *_udp_thread(void *arg) {
    (void) arg;
    sock_udp_ep_t remote = { .family = AF_INET, .port = UDP_SERVER_PORT };
    inet_pton(AF_INET, UDP_SERVER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return NULL;
    }

    while (true) {
        const char *state = "work";  // or "not work" depending on the actual state
        ssize_t res = sock_udp_send(&sock, state, strlen(state), &remote);
        
        if (res < 0) {
            printf("Error sending UDP packet\n");
        } else {
            printf("Sent state: %s\n", state);
        }

        ztimer_sleep(ZTIMER_USEC, STATE_UPDATE_INTERVAL * US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    printf("Starting UDP state sender\n");

    udp_thread_pid = thread_create(udp_thread_stack, sizeof(udp_thread_stack),
                                   THREAD_PRIORITY_MAIN - 1,
                                   THREAD_CREATE_STACKTEST,
                                   _udp_thread, NULL, "udp_sender");

    if (udp_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    return 0;
}