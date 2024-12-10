#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include <stdio.h>
#include <string.h>
#include "gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = SERVER_PORT
};

static char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static coap_pkt_t pdu;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo; // Suppress unused variable warning
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t *buf_ptr;
    int len;

    printf("Sending CoAP POST request to %s\n", SERVER_ADDR);

    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    buf_ptr = (uint8_t *)buf;

    len = gcoap_req_init(&pdu, buf_ptr, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);

    gcoap_req_send(buf_ptr, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}