#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void send_coap_request(void) {
    // Create a UDP socket
    sock_udp_ep_t remote = SOCK_IPV4_EP(SERVER_IP, SERVER_PORT);
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, &remote, 0);

    // Prepare the CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add the payload
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_pkt_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, coap_get_total_hdr_size(&pdu), &remote, NULL, NULL, SOCK_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close the UDP socket
    sock_udp_close(&sock);
}

int main(void) {
    // Initialize the network interface (assuming it is already connected)
    // This step depends on your specific setup and how you connect to the internet

    // Send the CoAP request
    send_coap_request();

    // Wait for a while before sending another request (optional)
    xtimer_sleep(10);

    return 0;
}