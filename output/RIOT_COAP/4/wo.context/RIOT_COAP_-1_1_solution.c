#include <stdio.h> 
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "thread.h"

#define COAP_PORT CONFIG_GCOAP_PORT
#define SERVER_IP "47.102.103.1"
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    printf("Response received\n");
}

int main(void) {
    // Buffer to hold the packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE));
    gcoap_finish(&pdu, len + sizeof(MESSAGE));

    // Set up remote endpoint
    sock_udp_ep_t remote = 
    {
        .family = AF_INET6,
        .port = COAP_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_IP);

    // Send the request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Main thread does nothing
    while (1) {
        thread_sleep();
    }

    return 0;
}