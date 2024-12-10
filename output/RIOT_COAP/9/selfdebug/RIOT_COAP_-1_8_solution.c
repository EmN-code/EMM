#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo; /* unused */
    (void)remote; /* unused */
    
    if (pdu->hdr->type == COAP_TYPE_ACK) {
        puts("Received ACK from server");
    } else {
        puts("No ACK received");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;
    
    memset(&remote, 0, sizeof(remote));
    
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    bytes_sent = gcoap_req_send(buf, pdu.payload_len + pdu.hdr->len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent successfully");
    
    return 0;
}