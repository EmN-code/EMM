#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include "msg.h"
#include "shell.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static const char *message = "Hello, COAP Cloud";

// buffer for CoAP communication
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

/* Response handler for incoming messages */
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu->hdr->code == COAP_CODE_205_CONTENT) {
        printf("Response received:\n");
        printf("%.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
    else {
        printf("Error in response\n");
    }
}

int main(void) {
    /* setup CoAP endpoint */
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    /* initialize CoAP request PDU */
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    /* set payload */
    size_t payload_len = strlen(message);
    memcpy(pdu.payload, message, payload_len);
    pdu.payload_len = payload_len;

    /* send CoAP request */
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res < 0) {
        printf("gcoap_req_send error\n");
    }
    else {
        printf("CoAP request sent\n");
    }

    return 0;
}