#include <string.h>
#include <stdio.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv4/addr.h"

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handle response */
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    int res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res <= 0) {
        printf("Error: invalid IP address\n");
        return -1;
    }

    remote.port = 5683;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    const char *msg = "Hello, COAP Cloud";
    len += coap_payload_put(&pdu, (uint8_t *)msg, strlen(msg));

    ssize_t bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error in sending CoAP request\n");
        return -1;
    }
    return 0;
}