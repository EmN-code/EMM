#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Function to send a CoAP POST request
static void send_coap_post_request(void) {
    // Create a CoAP packet buffer
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the CoAP packet
    int res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (res < 0) {
        printf("Error initializing CoAP request: %d\n", res);
        return;
    }

    // Add the payload to the CoAP packet
    res = coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    if (res < 0) {
        printf("Error adding format option: %d\n", res);
        return;
    }
    res = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (res < 0) {
        printf("Error finishing CoAP options: %d\n", res);
        return;
    }
    res = coap_pkt_add_payload(&pdu, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    if (res < 0) {
        printf("Error adding payload: %d\n", res);
        return;
    }

    // Create a UDP endpoint for the server
    sock_udp_ep_t server_ep = {
        .port = COAP_SERVER_PORT,
        .family = AF_INET,
        .addr = { .ipv4 = { .u32 = inet_addr(COAP_SERVER_IP) } }
    };

    // Send the CoAP request
    res = gcoap_req_send(buf, coap_pkt_len(&pdu), &server_ep, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
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

    // Wait for the network to be ready
    while (!gnrc_netif_is_up(netif)) {
        xtimer_sleep(1);
    }

    // Send the CoAP POST request
    send_coap_post_request();

    // Main loop
    while (1) {
        xtimer_sleep(10); // Sleep for 10 seconds
    }

    return 0;
}