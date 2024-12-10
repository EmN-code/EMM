#include "timex.h"
#include "thread_config.h"
#include "net/coap.h"
#include "net/gcoap.h"
#include "net/socket.h"
#include <stdio.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        printf("Response truncated\n");
    }
    else if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Request timed out\n");
    }
    else if (pdu != NULL) {
        printf("Success: received %u bytes\n", (unsigned)pdu->payload_len);
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6[0], COAP_SERVER_IP);

    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MSG, sizeof(COAP_MSG));
    len += sizeof(COAP_MSG);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}