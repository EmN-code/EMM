#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const void *arg, sock_udp_ep_t *remote, coap_pkt_t *pkt, size_t len) {
    printf("Response received: %.*s\n", (int)len, (char *)pkt->payload);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("inet_pton failed\n");
        return 1;
    }

    remote.port = SERVER_PORT;

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        printf("gcoap_req_send failed\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}