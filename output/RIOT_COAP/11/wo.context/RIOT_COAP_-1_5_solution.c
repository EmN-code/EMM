#include <net/sock/udp.h>
#include <net/sock/dtls.h>
#include <net/sock.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <stdio.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    if (pdu == NULL) {
        printf("gcoap: response timeout\n");
        return;
    }
    printf("gcoap: received response\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = EMQX_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    if (gcoap_req_send(&buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
        return -1;
    }
    printf("Request sent\n");

    return 0;
}