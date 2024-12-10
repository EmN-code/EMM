#include "string.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "kernel_defines.h"
#include <arpa/inet.h>
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) pdu;
    (void) remote;
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    size_t len = strlen(COAP_PAYLOAD);
    memcpy(pdu.payload, COAP_PAYLOAD, len);
    coap_opt_finish(&pdu, len);

    ssize_t res = gcoap_req_send(buf, pdu.used_size, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("gcoap_req_send() failed\n");
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}