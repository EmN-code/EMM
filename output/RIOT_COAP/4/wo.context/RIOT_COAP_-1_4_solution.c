#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling code here
    (void)memo;
    (void)pdu;
    (void)remote;
}

int main(void) {
    sock_udp_ep_t remote;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;  
    remote.port = 5683;

    // Correct the usage of inet_pton
    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);  
    if (res <= 0) {
        printf("inet_pton failed\n");
        return 1;
    }

    // Initialize CoAP PDU
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (result <= 0) {
        printf("Failed to initialize PDU\n");
        return 1;
    }

    // Set payload data
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send the request
    result = gcoap_req_send(&buf[0], pdu.opt_num, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("Failed to send request\n");
        return 1;
    }

    return 0;
}
