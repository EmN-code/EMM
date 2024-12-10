
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <unistd.h>
#include <dht_params.h>
#include <net/sock/tcp.h>
#include <time_units.h>
#include <esp_wifi.h>
#include <net/gcoap.h>
#include <stdbool.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUFFER_SIZE 128
#define MSG_QUEUE_SIZE 10

static sock_tcp_t sock;
static char buffer[BUFFER_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void publish_state(void) {
    const char *state = "work or not work";
    int res = sock_tcp_send(&sock, state, strlen(state), 0);
    if (res < 0) {
        puts("Failed to send data");
    } else {
        puts("Data sent successfully");
    }
}

void *worker_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        publish_state();
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to create socket");
        return 1;
    }

    pthread_t worker;
    pthread_create(&worker, NULL, worker_thread, NULL);

    while (1) {
        msg_t msg;
        msg.content.value = 0;
        mbox_put(&mbox, &msg);
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}
