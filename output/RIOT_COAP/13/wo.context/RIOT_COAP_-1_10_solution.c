#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <net/ipv4/addr.h>
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define BUF_SIZE 128

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP request sent to %s:%d with path %s\n", COAP_SERVER_IP, COAP_SERVER_PORT, COAP_URI_PATH);

    return 0;
}