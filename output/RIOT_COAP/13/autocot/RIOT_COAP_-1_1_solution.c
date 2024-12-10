#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include "gcoap.h"
#include <stdint.h>
#include <stdio.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_COMPLETED) {
        printf("Response received\n");
    } else {
        printf("Error code: %d\n", memo->state);
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, COAP_SERVER_IP);
    
    char *message = "Hello, COAP Cloud";
    
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, message, strlen(message));
    pdu.payload_len = strlen(message);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}