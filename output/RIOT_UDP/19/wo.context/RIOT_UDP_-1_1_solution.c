#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <thread.h>
#include <net/af.h>
#include <net/conn/udp.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define LOCAL_PORT 3000
#define SEND_INTERVAL (5U * 1000000U)

static const char *device_state = "work";

void *udp_sender(void *arg) {
    (void)arg;
    uint8_t buf[128];
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_ep_t local = { .family = AF_INET6, .port = LOCAL_PORT };

    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR);

    int res = sock_udp_create(NULL, &local, NULL, 0);
    if (res < 0) {
        printf("Error: Could not create UDP socket\n");
        return NULL;
    }
    
    while (1) {
        res = sock_udp_send(NULL, device_state, strlen(device_state), &remote);
        if (res < 0) {
            printf("Error: Could not send message\n");
        } else {
            printf("Device state sent: %s\n", device_state);
        }
        xtimer_sleep(SEND_INTERVAL / 1000000U);
    }
    return NULL;
}

int main(void) {
    printf("Starting UDP client\n");
    thread_create((char *)udp_sender_thread_stack, sizeof(udp_sender_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, udp_sender, NULL, "udp_sender");
    return 0;
}