
#include <stdio.h>
#include <stdlib.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
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
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <xtimer.h>
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
    uint8_t payload[] = MQTT_PAYLOAD;
    sock_tcp_send(&sock, payload, MQTT_PAYLOAD_LEN);
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
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error: invalid address");
        return 1;
    }

    if (sock_tcp_create(&sock, NULL, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: failed to create socket");
        return 1;
    }

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    kernel_pid_t pid = thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, worker_thread, NULL, "worker");

    evtimer_msg_event_t event;
    evtimer_msg_event_init(&event, pid, MSG_TYPE_PUBLISH);
    evtimer_add_msg(&mbox, &event.event, INTERVAL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        evtimer_add_msg(&mbox, &event.event, INTERVAL);
    }

    sock_tcp_close(&sock);
    return 0;
}
