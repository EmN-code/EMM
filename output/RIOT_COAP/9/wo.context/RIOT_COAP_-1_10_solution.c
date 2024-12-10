#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define PAYLOAD_LENGTH 17

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received: %s\n", (char *)pdu->payload);
}

int main(void) {
    coap_pkt_t pdu;
    int result;

    /* Convert IP address string to network byte order */
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Unable to parse IP address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    /* Initialize CoAP PDU */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, PAYLOAD_LENGTH);
    pdu.payload_len = PAYLOAD_LENGTH;

    /* Send CoAP request */
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}