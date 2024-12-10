#include <net/sock/udp.h>
#include <net/coap.h>
#include <net/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handler function just logs the response */
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    /* Convert string IP to a usable format */
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result == 0) {
        printf("Malformed IP address\n");
        return -1;
    }

    /* Initialize CoAP packet with POST request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    /* Add payload to the packet */
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD) - 1);  // exclude null terminator

    /* Send CoAP message */
    gcoap_req_send(buf, sizeof(PAYLOAD) - 1 + coap_hdr_len(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP POST request sent to %s\n", SERVER_IP);

    return 0;
}