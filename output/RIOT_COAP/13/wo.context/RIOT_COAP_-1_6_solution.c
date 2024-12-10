#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/af.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
   
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}