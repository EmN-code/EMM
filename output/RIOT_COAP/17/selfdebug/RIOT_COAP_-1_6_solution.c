#include <net/ipv6/addr.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/sock/udp.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received from server\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    printf("Sending CoAP POST request to %s:%d\n", SERVER_IP, SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: Failed to send CoAP request\n");
        return -1;
    }

    return 0;
}