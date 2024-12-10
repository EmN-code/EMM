#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t server;
    server.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, SERVER_ADDR);
    server.port = SERVER_PORT;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += coap_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));

    if (gcoap_req_send(buf, len, &server, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("[Error]: Failed to send CoAP request\n");
        return 1;
    }

    printf("[Success]: CoAP request sent to %s at port %d\n", SERVER_ADDR, SERVER_PORT);
    return 0;
}