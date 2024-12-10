#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void send_coap_post_request(void) {
    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    // Convert the server IP address to a binary format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Failed to parse server IP address\n");
        return;
    }

    // Create the UDP socket
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return;
    }

    // Prepare the CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add the payload to the CoAP request
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_pkt_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", (int)res);
    } else {
        printf("CoAP POST request sent successfully\n");
    }

    // Close the UDP socket
    sock_udp_close(&sock);
}

int main(void) {
    // Initialize the network interface (if needed)
    // This step depends on your specific setup and how you connect to the internet

    // Send the CoAP POST request
    send_coap_post_request();

    // Wait for a while before exiting
    xtimer_sleep(5);

    return 0;
}