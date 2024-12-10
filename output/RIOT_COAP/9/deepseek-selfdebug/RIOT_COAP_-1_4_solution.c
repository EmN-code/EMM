
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *resp = (char *)pdu->payload;
        resp[pdu->payload_len] = '\0';
        printf("Received response: %s\n", resp);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Initialize remote endpoint
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    // Initialize CoAP packet
    result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
