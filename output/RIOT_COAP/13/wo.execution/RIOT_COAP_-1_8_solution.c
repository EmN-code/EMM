#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static ssize_t _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    size_t payload_len;
    const uint8_t *payload = coap_get_payload(pdu, &payload_len);
    printf("Response: %.*s\n", (int)payload_len, (char *)payload);
    return 0;
}

int main() {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    const uint8_t *data = (uint8_t *)"Hello, COAP Cloud";
    coap_pkt_append_payload(&pdu, data, strlen((char *)data));

    gcoap_req_send(pdu.hdr, pdu.total, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    return 0;
}