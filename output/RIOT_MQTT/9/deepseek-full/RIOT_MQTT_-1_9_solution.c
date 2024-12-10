
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/tcp.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <timex.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MSG_TYPE_PUBLISH 1
#define MSG_TYPE_CONNECT 2
#define MSG_TYPE_DISCONNECT 3

static sock_tcp_t sock;
static msg_t msg_queue[16];
static mbox_t mbox;
static evtimer_msg_t evtimer;
static evtimer_msg_event_t ev_event;

void publish_state(void) {
    const char *state = "work"; // or "not work" based on your logic
    sock_tcp_send(&sock, state, strlen(state));
}

void event_callback(void *arg) {
    msg_t msg;
    msg.type = MSG_TYPE_PUBLISH;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    evtimer_add_msg(&evtimer, &ev_event, thread_getpid());

    // Connect to the MQTT broker
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &server_addr.sin_addr);

    sock_tcp_create(&sock, NULL, (struct sockaddr *)&server_addr, SOCK_FLAGS_REUSE_EP);

    // Main loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);

        switch (msg.type) {
            case MSG_TYPE_PUBLISH:
                publish_state();
                break;
            default:
                break;
        }
    }

    // Close the socket
    sock_tcp_close(&sock);

    return 0;
}
