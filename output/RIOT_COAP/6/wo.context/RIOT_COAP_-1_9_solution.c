#include <net/gcoap.h>
#include <stdio.h>
#include <msg.h>
#include <kernel_types.h>
#include <stdlib.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

int main(void) {
    // Prepare the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize a CoAP request
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH) == -1) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Set payload
    ssize_t hdrlen = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (hdrlen < 0) {
        printf("Error finishing options\n");
        return 1;
    }
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    ssize_t payload_len = sizeof(COAP_MESSAGE);
    ssize_t len = gcoap_req_send(buf, hdrlen + payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP message sent successfully to %s:%d\n", COAP_SERVER_IP, COAP_SERVER_PORT);
    return 0;
}