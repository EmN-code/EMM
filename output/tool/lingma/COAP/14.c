#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

// Define the server's IP address and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

// Function to handle the CoAP response
static void handle_response(void *context, coap_pkt_t *pdu, sock_udp_ep_t *remote) {
    (void)context;
    (void)remote;

    // Print the response code
    printf("Received response code: %u.%02u\n", coap_get_code_class(pdu), coap_get_code_detail(pdu));

    // Print the response payload if any
    if (coap_get_payload(pdu)) {
        printf("Response payload: %.*s\n", (int)pdu->payload_len, (char *)coap_get_payload(pdu));
    }
}

// Main function
int main(void) {
    // Initialize the network
    sock_udp_ep_t server_ep = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    inet_pton(AF_INET, SERVER_IP, &server_ep.addr);

    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &server_ep, 0);

    // Prepare the CoAP POST request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add the payload to the CoAP request
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_add_payload(&pdu, strlen(PAYLOAD), (uint8_t *)PAYLOAD);

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &server_ep, handle_response, NULL, GNRC_NETTYPE_IPV4);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", (int)res);
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Wait for the response
    xtimer_sleep(10); // Adjust the sleep time as needed

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}