#include <thread.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <net/gnrc/netif.h>
#include <stdio.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static void *udp_sender(void *arg) {
    (void)arg;  // Unused parameter
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };

    if (sock_udp_str2ep(&server, SERVER_ADDR) < 0) {
        printf("Error: unable to parse destination address.\n");
        return NULL;
    }

    while (1) {
        if (sock_udp_send(NULL, DEVICE_STATE, sizeof(DEVICE_STATE), &server) < 0){
            printf("Error: unable to send data.\n");
        } else {
            printf("Message sent: %s\n", DEVICE_STATE);
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t udp_sender_pid = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, udp_sender, NULL, "udp_sender");
    if (udp_sender_pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread\n");
        return 1;
    }
    return 0;
}