
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <mbox.h>
#include <msg.h>
#include <net/sock/tcp.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MSG_QUEUE_SIZE 16
#define MSG_TYPE_PUBLISH 1
#define MSG_TYPE_RECEIVE 2

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static sock_tcp_t sock;

void publish_state(void) {
    char state[] = "work";
    ssize_t res = sock_tcp_send(&sock, state, sizeof(state), 0);
    if (res < 0) {
        puts("Failed to send state");
    } else {
        puts("State published");
    }
}

void *worker_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_PUBLISH) {
            publish_state();
        }
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    kernel_pid_t pid = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");
    if (pid <= 0) {
        puts("Failed to create worker thread");
        sock_tcp_close(&sock);
        return 1;
    }

    evtimer_mbox_add_msg(&mbox, 5 * US_PER_SEC, MSG_TYPE_PUBLISH, NULL, 0);

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_RECEIVE) {
            puts("Message received");
        }
    }

    sock_tcp_close(&sock);
    return 0;
}
