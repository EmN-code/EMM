
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char resp[BUF_SIZE];
        size_t len = gcoap_get_payload(pdu, (uint8_t **)&resp);
        resp[len] = '\0';
        printf("Received: %s\n", resp);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid address");
        return 1;
    }

    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;

    ssize_t len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
