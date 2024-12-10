#include "thread.h"
#include "mutex.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "esp_system.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"

#define SERVER_ADDRESS "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL 5
#define STACK_SIZE (THREAD_STACKSIZE_MAIN + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACK_SIZE];
static kernel_pid_t sender_pid;

void *udp_sender_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };
    sock_udp_t sock;

    if (inet_pton(AF_INET6, SERVER_ADDRESS, &server.addr.ipv6) != 1) {
        puts("Error: Cannot parse server address");
        return NULL;
    }

    if (sock_udp_create(&sock, NULL, &server, 0) < 0) {
        puts("Error: Cannot create UDP socket");
        return NULL;
    }

    const char *state = "work";
    while (1) {
        if (sock_udp_send(&sock, state, strlen(state), &server) < 0) {
            puts("Error sending data");
        } else {
            puts("Sent data successfully");
        }
        thread_sleep(SEND_INTERVAL);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                               udp_sender_thread, NULL, "udp_sender");
    return 0;
}