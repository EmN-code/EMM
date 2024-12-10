#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "gcoap.h"
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("No response received\n");
        return -1; // Retry mechanism can be implemented here
    }
    if (pdu) {
        printf("Response received\n");
        return 0;
    }
    return -1;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    uint8_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
    }
    return 0;
}
