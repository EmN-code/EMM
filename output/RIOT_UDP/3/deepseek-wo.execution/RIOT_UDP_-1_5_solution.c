
#include "time_units.h"
#include "stdio.h"
#include "mutex.h"
#include "net/gcoap.h"
#include "unistd.h"
#include "msg.h"
#include "timex.h"
#include "dht.h"
#include "ztimer.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "net/sock/udp.h"
#include "mbox.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "esp_wifi.h"
#include "evtimer_msg.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "dht_params.h"
#include "xtimer.h"
#include "net/sock.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define MSG_QUEUE_SIZE 10
#define DEVICE_STATE "work"  // or "not work"

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void send_state(void) {
    char state[10];
    strcpy(state, DEVICE_STATE);
    sock_udp_send(&sock, state, strlen(state), &remote);
}

int main(void) {
    msg_init_queue(_main_msg_queue, MSG_QUEUE_SIZE);

    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Create the UDP socket
    sock_udp_create(&sock, &local, &remote, 0);

    while (1) {
        send_state();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
