#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

#define BUFFER_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static gcoap_request_memo_t *req_memo;
static uint8_t pdu_buf[BUFFER_SIZE];

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        puts("Remote endpoint does not respond\n");
        return;
    }
    printf("Response received: %u\n", coap_get_code(pdu));
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse server address\n");
        return 1;
    }

    ssize_t result;
    uint16_t payload_offset;

    gcoap_req_init(&pdu, pdu_buf, sizeof(pdu_buf), COAP_METHOD_POST, URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);

    payload_offset = coap_get_payload(pdu, &pdu_buf);
    strcpy((char *)&pdu_buf[payload_offset], "Hello, COAP Cloud");

    result = gcoap_req_send(pdu_buf, coap_hdr_get_len(&pdu), &remote, _response_handler, &req_memo, GCOAP_SOCKET_TYPE_UDP);
    if (result <= 0) {
        puts("Error: failed to send request\n");
        return 1;
    }

    return 0;
}