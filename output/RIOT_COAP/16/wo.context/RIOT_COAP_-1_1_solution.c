#include <stdio.h>
#include <string.h>
#include <net/nanocoap.h>
#include <gcoap.h>
#include <sock_udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        printf("Warning: response truncated\n");
    }
    else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: request timed out\n");
    }
    else {
        printf("Response received\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    sock_udp_ep_t remote = { .family = AF_INET6, .netmask_len = 128 };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");
    return 0;
}