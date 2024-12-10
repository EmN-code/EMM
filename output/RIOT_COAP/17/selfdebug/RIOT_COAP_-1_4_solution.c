#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/sock/tcp.h"
#include "net/sock/dtls.h"
#include "net/sock.h"
#include "net/ipv4/addr.h"

#include <stdio.h>
#include <string.h>

#define COAP_PORT 5683
#define SERVER_IP "47.102.103.1"
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send CoAP request
    len = gcoap_req_send(buf, sizeof(buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");
    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response here.
    printf("Received response\n");
}