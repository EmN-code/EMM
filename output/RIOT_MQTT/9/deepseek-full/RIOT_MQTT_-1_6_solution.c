
#include <evtimer_msg.h>
#include <stdio.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <unistd.h>
#include <net/sock/tcp.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <stdbool.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_PAYLOAD_LEN 4
#define INTERVAL (5 * US_PER_SEC)

static sock_tcp_t sock;
static mbox_t mbox;
static msg_t msg_queue[8];

void publish_state(void) {
    char payload[] = MQTT_PAYLOAD;
    sock_tcp_send(&sock, payload, MQTT_PAYLOAD_LEN);
}

void *worker_thread(void *arg) {
    (void)arg;
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        publish_state();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating socket");
        return 1;
    }

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_t *worker;
    worker = thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                           THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    evtimer_mbox_add_msg(&mbox, INTERVAL, worker);

    while (1) {
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
