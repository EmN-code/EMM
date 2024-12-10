#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define CONFIG_GCOAP_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("GCOAP Request timeout\n");
        return;
    }
    printf("Successful GCOAP response\n");
}

int main(void) {
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));

    remote.family = AF_INET;
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return 1;
    }
    remote.port = CONFIG_GCOAP_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}