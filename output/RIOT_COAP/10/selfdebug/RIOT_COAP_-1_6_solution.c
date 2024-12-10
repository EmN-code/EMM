#include <string.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <arpa/inet.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;  // To avoid unused parameter warnings
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("CoAP request timed out\n");
    } else {
        printf("CoAP response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    size_t len;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert server IP address\n");
        return -1;
    }

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        printf("CoAP request sent: %s\n", PAYLOAD);
    } else {
        printf("Error sending CoAP request\n");
    }

    return 0;
}