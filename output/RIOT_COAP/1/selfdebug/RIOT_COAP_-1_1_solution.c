#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_STATE_RESP) {  
        printf("Response received\n");
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    char payload[] = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, sizeof(payload));
    pdu.payload_len = sizeof(payload);

    gcoap_req_send(buf, pdu.payload_len + gcoap_opt_finish(&pdu, COAP_OPT_FINISH_NONE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}