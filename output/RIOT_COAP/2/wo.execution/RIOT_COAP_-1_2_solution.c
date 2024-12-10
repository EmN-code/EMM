#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <net/gcoap.h>

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    /* handle response */
}

int main(void) {
    coap_pkt_t pdu;
    const char *server_ip = "47.102.103.1";

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = CONFIG_GCOAP_PORT;

    if (inet_pton(AF_INET, server_ip, &remote.addr.ipv4) != 1) {
        /* handle error */
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);

    size_t len = gcoap_finish(&pdu, 17, 0);
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}