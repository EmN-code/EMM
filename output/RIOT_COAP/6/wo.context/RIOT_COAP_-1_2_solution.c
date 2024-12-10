#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

int main(void) {
    ipv6_addr_t addr;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };

    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr) != 1) {
        puts("Error converting IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    len += strlen("Hello, COAP Cloud");
 
    ssize_t res = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res >= 0) {
        puts("CoAP request sent successfully");
    } else {
        printf("Error sending CoAP request: %d\n", (int)res);
    }

    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}
