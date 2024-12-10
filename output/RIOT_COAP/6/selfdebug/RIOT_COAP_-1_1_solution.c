#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"

#define SERVER_IPV6_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("CoAP Response received from %s\n", SERVER_IPV6_ADDR);
    } else {
        printf("CoAP Request timeout\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    /* Prepare CoAP server endpoint */
    remote.family = AF_INET6;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_IPV6_ADDR);

    /* Initialize CoAP request */
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH) < 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    /* Add message to CoAP PDU */
    if (coap_opt_add_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE)) < 0) {
        printf("Failed to add payload to CoAP PDU\n");
        return -1;
    }
    
    /* Send CoAP request */
    bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }
    
    printf("CoAP POST request sent: %s\n", MESSAGE);
    return 0;
}