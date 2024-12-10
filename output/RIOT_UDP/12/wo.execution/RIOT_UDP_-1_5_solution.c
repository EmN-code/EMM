#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "dht.h"
#include "esp_system.h"
#include "net/af.h"
#include <net/emcute.h>
#include "arpa/inet.h"
#include "sys/socket.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define SEND_INTERVAL (5U)

int main(void) {
    // Define remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    
    // Initialize the socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        return 1; // Failed to create socket
    }
    
    // Buffer to hold the device state message
    const char *state_msg = "work";
    ssize_t msg_len = strlen(state_msg);

    while (1) {
        // Send the message to the server
        if (sock_udp_send(&sock, state_msg, msg_len, &remote) < 0) {
            // Handle error
            continue;
        }

        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL * 1000);
    }

    return 0;
}