#include <net/sock/udp.h>
#include <net/ipv6/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_PORT 5683

int gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = COAP_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");

    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    gcoap_req_send(buf, sizeof(buf), &remote, NULL, 0, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}