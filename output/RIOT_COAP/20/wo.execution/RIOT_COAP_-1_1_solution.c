#include "net/gcoap.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

int main(void) {
    printf("Starting CoAP client\n");

    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t server = { .family = AF_INET6, .port = SERVER_PORT };

    /* Parse server address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_IP) == NULL) {
        puts("Error in parsing server IP");
        return -1;
    }

    /* Initialize PDU for CoAP POST request */
    size_t len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE) - 1);
    pdu.payload_len = sizeof(MESSAGE) - 1;
    
    /* Send CoAP request */
    if (gcoap_req_send(buf, len, &server, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) == 0) {
        puts("CoAP request sent");
    }
    else {
        puts("Error sending CoAP request");
    }

    return 0;
}