#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST COAP_METHOD_POST

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    const char *payload = "Hello, COAP Cloud";
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    remote.port = CONFIG_GCOAP_PORT;
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if(res < 0) {
        printf("Error sending request\n");
    }
    else {
        printf("Request sent successfully\n");
    }

    return 0;
}