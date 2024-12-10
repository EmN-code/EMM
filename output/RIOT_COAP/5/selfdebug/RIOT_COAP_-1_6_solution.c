#include <arpa/inet.h>
#include <net/ipv6/addr.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include <string.h>
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; // unused
    if (pdu == NULL) {
        puts("No response received");
    } else {
        printf("Response received COAP Code: %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };

    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (len < 0) {
        puts("Error initializing request");
        return 1;
    }

    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    len += sizeof("Hello, COAP Cloud") - 1;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending request");
        return 1;
    }

    puts("Request sent");
    return 0;
}