#include "net/nanocoap/cache.h"
#include "coap_pkt.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    puts("Response received");
    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP);

    puts("Sending CoAP POST request...");

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    strcpy((char *)pdu.payload, "Hello, COAP Cloud");
    len += strlen("Hello, COAP Cloud");
    coap_opt_finish(&pdu, len);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) == 0) {
        puts("Request sent");
    } else {
        puts("Failed to send request");
    }

    return 0;
}