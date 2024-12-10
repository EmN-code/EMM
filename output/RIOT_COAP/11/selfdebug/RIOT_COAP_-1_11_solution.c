#include <net/sock/udp.h>
#include <gcoap.h>
#include <coap.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02 // Define POST as 0x02 according to CoAP specs

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response here
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    
    remote.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = 5683;
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    // Send the request
    gcoap_req_send(buf, pdu.hdr->payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
