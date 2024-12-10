#include "thread.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "sys/socket.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 20001
#define STACK_SIZE  (THREAD_STACKSIZE_MAIN)
#define SLEEP_DURATION (5000U)  // in milliseconds

static char udp_sender_stack[STACK_SIZE];
static kernel_pid_t udp_sender_pid;

void *udp_sender_thread(void *arg) {
    (void)arg;

    // Setup UDP socket
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return NULL;
    }
    memcpy(&remote.addr.ipv6, &addr, sizeof(addr));

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    char *state_msg = "work or not work";

    while (1) {
        if (sock_udp_send(&sock, state_msg, strlen(state_msg), &remote) < 0) {
            puts("Error: unable to send message");
        } else {
            puts("Message sent successfully");
        }
        thread_sleep(SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    // Start the UDP sending thread
    udp_sender_pid = thread_create(udp_sender_stack, sizeof(udp_sender_stack),
                                  THREAD_PRIORITY_MAIN - 1, 0,
                                  udp_sender_thread, NULL, "udp_sender");
    if (udp_sender_pid <= 0) {
        puts("Error: unable to create thread");
        return 1;
    }
    return 0;
}