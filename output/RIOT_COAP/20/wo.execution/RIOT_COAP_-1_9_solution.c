#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/nanocoap/cache.h"
#include <sys/socket.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>
#include <sched.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

#define COAP_PORT 5683
#define COAP_METHOD_POST 0x02
#define BUF_SIZE 256
#define PAYLOAD "Hello, COAP Cloud"

// CoAP handler
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    printf("Response received (Code: %1u.%02u): %u\n", COAP_GET_CODE_CLASS(pdu->hdr->code),
           COAP_GET_CODE_DETAIL(pdu->hdr->code), (unsigned)pdu->payload_len);
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };
    if (inet_pton(AF_INET6, "47.102.103.1", &remote.addr.ipv6) <= 0) {
        puts("Error: unable to parse destination address");
        return -1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    if (len < 0) {
        puts("Error: unable to initialize request");
        return -1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}