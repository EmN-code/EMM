#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"

#define SERVER_IP           "47.102.103.1"
#define SERVER_PORT         5683
#define URI_PATH            "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo,
                          coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: invalid IP address");
        return 1;
    }

    /* Initialize CoAP PDU */
    gcoap_req_init(NULL, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    snprintf((char *)buf + 4, sizeof(buf) - 4, "%s", "Hello, COAP Cloud");

    /* Send CoAP request */
    gcoap_req_send(buf, strlen((char *)buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}