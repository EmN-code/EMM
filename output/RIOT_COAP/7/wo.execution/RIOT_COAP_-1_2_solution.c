#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/nanocoap.h"
#include "net/mqttsn.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"

#include <stdio.h>
#include <string.h>

#define COAP_SERVER "47.102.103.1"
#define COAP_PORT 5683
#define COAP_PATH "/coap/test"

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("CoAP request timed out\n");
        return;
    }
    printf("Received response: Code %u\n", pdu ? pdu->hdr->code : 0);
}

int main(void) {
    uint8_t buf[256];
    ssize_t bytes_sent;
    sock_udp_ep_t server = { .family = AF_INET, .port = COAP_PORT };
    inet_pton(AF_INET, COAP_SERVER, &server.addr.ipv6[12]);

    coap_pkt_t pdu;
    size_t len = sizeof(buf);
    int res = gcoap_req_init(&pdu, buf, len, COAP_METHOD_POST, COAP_PATH);
    if (res < 0) {
        printf("Error initializing request\n");
        return res;
    }

    memcpy(buf + len, "Hello, COAP Cloud", 17);

    bytes_sent = gcoap_req_send(buf, len + 17, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("Request sent successfully\n");

    return 0;
}