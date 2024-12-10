#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <string.h>

#define SERVER_IP     "47.102.103.1"
#define COAP_PORT     5683
#define URI_PATH      "/coap/test"
#define PAYLOAD       "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_STATE_COMPLETED) {
        printf("Response received: %u bytes\n", pdu->payload_len);
    }
    else {
        printf("Error in response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;
    sock_udp_ep_t remote;

    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
    }

    return 0;
}