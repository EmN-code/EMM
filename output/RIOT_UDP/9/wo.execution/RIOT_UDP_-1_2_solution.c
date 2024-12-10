#include "thread.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "event/thread.h"

#define DEFAULT_PORT 20001
#define STACK_SIZE_THREAD  (THREAD_STACKSIZE_MAIN)
#define UDP_SERVER "47.102.103.1"
#define MESSAGE_INTERVAL (5U)

static kernel_pid_t main_thread_pid;
static char stack[STACK_SIZE_THREAD];

void *send_udp(void *arg) {
    (void) arg;
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = DEFAULT_PORT };
    uint8_t state_msg[] = "work";

    /* start a Socket */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        return NULL;
    }

    /* resolve the hostname and set the remote endpoint */
    if (sock_udp_endpoint_resolve(&remote, UDP_SERVER) < 0) {
        sock_udp_close(&sock);
        return NULL;
    }

    while (1) {
        /* send the state message to the server */
        sock_udp_send(&sock, state_msg, sizeof(state_msg), &remote);
        /* wait for 5 seconds before sending the next message */
        xtimer_sleep(MESSAGE_INTERVAL);
    }

    sock_udp_close(&sock);
    return NULL;
}

int main(void) {
    main_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
            THREAD_CREATE_STACKTEST, send_udp, NULL, "udp_sender");
    
    thread_sleep();
    return 0;
}