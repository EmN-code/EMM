c
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Function to handle the CoAP response
static void coap_response_handler(void *arg, ssize_t result, const uint8_t *buf, size_t len) {
    if (result < 0) {
        printf("CoAP request failed: %zd\n", result);
    } else {
        printf("CoAP response received:\n");
        for (size_t i = 0; i < len; i++) {
            printf("%02x ", buf[i]);
        }
        printf("\n");
    }
}

// Function to send a CoAP POST request
static void send_coap_post_request(void) {
    // Create a UDP endpoint for the CoAP server
    sock_udp_ep_t server_ep = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };
    inet_pton(AF_INET, COAP_SERVER_IP, &server_ep.addr);

    // Initialize the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add the payload to the CoAP packet
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_pkt_add_payload(&pdu, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.len, &server_ep, coap_response_handler, NULL, GCOAP_TRANSPORT_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request: %zd\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void) {
    // Initialize the network stack
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(GNRC_NETIF_DEFAULT_PID);
    if (!netif) {
        printf("Failed to initialize network interface\n");
        return -1;
    }

    // Initialize the CoAP library
    gcoap_init();

    // Send the CoAP POST request
    send_coap_post_request();

    // Wait for the response
    xtimer_sleep(5); // Adjust the sleep duration as needed

    return 0;
}