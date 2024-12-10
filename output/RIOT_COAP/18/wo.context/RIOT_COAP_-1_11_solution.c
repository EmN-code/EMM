#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *response_pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_UNUSED) {
        printf("Error in CoAP response\n");
        return;
    }
    printf("Received CoAP response\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    /* Initialize the remote endpoint */
    memset(&remote, 0, sizeof(remote));
    remote.port = 5683;
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    /* Initialize CoAP request PDU */
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len <= 0) {
        puts("Error: unable to initialize PDU");
        return 1;
    }

    /* Set the payload for the POST request */
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    /* Send the CoAP request */
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (!res) {
        puts("Error: unable to send CoAP request");
        return 1;
    }

    puts("CoAP request sent successfully\n");
    return 0;
}