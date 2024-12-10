#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

int main(void) {
    // Initialize the network interface
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT,
    };
    inet_pton(AF_INET, COAP_SERVER_IP, &server.addr);

    // Create a UDP socket
    sock_udp_t sock;
    sock_udp_create(&sock, NULL, NULL, 0);

    // Initialize the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add the payload
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_add_payload(&pdu, COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    // Send the CoAP request
    ssize_t res = sock_udp_send(&sock, buf, coap_get_total_hdr_size(&pdu), &server);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close the UDP socket
    sock_udp_close(&sock);

    return 0;
}