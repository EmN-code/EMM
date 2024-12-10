#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Function to send a CoAP POST request
static void send_coap_post_request(void) {
    // Create a UDP endpoint for the server
    sock_udp_ep_t server_ep = {
        .port = COAP_SERVER_PORT,
        .addr = { .ipv4 = { .addr = inet_addr(COAP_SERVER_IP) } }
    };

    // Buffer for the CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Initialize the CoAP PDU
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add the payload to the CoAP PDU
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_add_payload(&pdu, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send the CoAP PDU
    ssize_t res = gcoap_req_send(buf, coap_get_total_hdr_size(&pdu), &server_ep, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        printf("Failed to send CoAP POST request: %d\n", (int)res);
    } else {
        printf("CoAP POST request sent successfully\n");
    }
}

int main(void) {
    // Initialize the network stack
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(0); // Assuming the first network interface
    if (!netif) {
        printf("No network interface found\n");
        return -1;
    }

    // Initialize the gcoap library
    kernel_pid_t gcoap_pid = gcoap_init();
    if (gcoap_pid < 0) {
        printf("Failed to initialize gcoap\n");
        return -1;
    }

    // Send the CoAP POST request
    send_coap_post_request();

    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}