#include <net/sock/util.h>
#include <periph/gpio.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <xtimer.h>
#include <timex.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_PATH "/coap/test"

int main(void) {
    /* Create UDP socket */
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error: Unable to create UDP socket\n");
        return -1;
    }

    /* Construct CoAP packet manually */
    uint8_t buf[64];
    size_t len = coap_build(buf, sizeof(buf), "HELLO, COAP CLOUD", COAP_PATH, "post");

    if (len < 0) {
        puts("Error: Failed to build CoAP packet\n");
        return -1;
    }

    /* Send CoAP request */
    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error: Unable to send CoAP message\n");
        return -1;
    }

    puts("CoAP POST request sent\n");
    return 0;
}

size_t coap_build(uint8_t *buf, size_t buf_len, const char *payload, const char *path, const char *method) {
    /* Function to manually assemble the CoAP packet */
    if (strcmp(method, "post") != 0) {
        return -1;
    }

    /* Placeholder: Assemble CoAP packet (header + payload) */
    /* CoAP header, token, options setup not included for brevity */
    /* Assume function sets up CoAP POST header and adds path and payload */

    if (strlen(payload) + 4 > buf_len) { // Simplified header length check
        return -1;
    }

    /* Copying payload to buf for demonstration, full CoAP packet setup requires specific details */
    memcpy(buf + 4, payload, strlen(payload));
    return strlen(payload) + 4; // Simplified return value (header+payload length)
}