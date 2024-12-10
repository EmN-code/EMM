#include "net/sock/udp.h"
#include "net/sock.h"
#include "net/gcoap.h"

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout received\n");
        return;
    }
    if (pdu->payload_len) {
        printf("Response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    /* Initialize remote endpoint structure */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* Add payload to CoAP packet */
    gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}