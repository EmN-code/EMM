#include <string.h>
#include <net/sock/udp.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(unsigned req_state, coap_pkt_t* /* gcoap_request_memo_t and coap_pkt_t not defined in provided context */ pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("CoAP request timeout\n");
    }
    else {
        printf("Response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}