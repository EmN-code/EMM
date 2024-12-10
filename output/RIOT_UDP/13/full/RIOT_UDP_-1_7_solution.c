#include "event/thread.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mutex.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "ztimer.h"

int main() {
    // Network configuration
    sock_udp_ep_t server = { .family = AF_INET, .port = 20001 };
    inet_pton(AF_INET, "47.102.103.1", &server.addr.ipv4);

    sock_udp_t sock;
    char *data = "Device is working";  // Define device state
    size_t data_len = strlen(data);
    int rc;

    // Create UDP socket
    rc = sock_udp_create(&sock, NULL, NULL, 0);
    if (rc < 0) {
        fprintf(stderr, "Error creating UDP socket\n");
        return -1;
    }

    // Send device state every five seconds
    while (1) {
        if (sock_udp_send(&sock, data, data_len, &server) < 0) {
            fprintf(stderr, "Error sending UDP packet\n");
        } else {
            fprintf(stdout, "Device state sent successfully\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }

    return 0;
}