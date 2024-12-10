#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net/nanocoap.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "xtimer.h"
#include "msg.h"
#include "net/sock/udp.h"

#define SERVER_EP "coap://47.102.103.1:5683"
#define EMQX_URI_PATH "/coap/test"

static size_t _send_coap_request(const char *message) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    gcoap_request_memo_t memo;
    sock_udp_ep_t remote;

    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }
    remote.port = 5683;

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);

    gcoap_req_send(buf, len, &remote, NULL, &memo);
    return 0;
}

int main(void) {
    printf("Sending CoAP POST request to the server\n");
    char *message = "Hello, COAP Cloud";
    if (_send_coap_request(message) == 0) {
        printf("CoAP request sent successfully\n");
    }
    else {
        printf("Failed to send CoAP request\n");
    }
    return 0;
}