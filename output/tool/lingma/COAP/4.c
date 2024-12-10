#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

// Define the server's IP address and port
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

// Function to send a CoAP POST request
static void send_coap_post_request(void) {
    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Create the socket
    int res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create socket: %d\n", res);
        return;
    }

    // Prepare the CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add the payload
    const char *payload = "Hello, COAP Cloud";
    len += coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(buf + len, payload, strlen(payload));
    len += strlen(payload);

    // Send the CoAP request
    res = sock_udp_send(&sock, buf, len, &remote);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", res);
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void) {
    // Initialize the network stack
    gnrc_netif_init();

    // Send the CoAP POST request
    send_coap_post_request();

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}