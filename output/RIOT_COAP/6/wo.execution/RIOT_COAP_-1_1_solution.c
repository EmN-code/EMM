#include "net/nanocoap/cache.h"
#include "thread_config.h"
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/gnrc/ipv6/nib.h"

#define SERVER_IP "47.102.103.1"
#define COAP_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout");
        return GCOAP_MEMO_TIMEOUT;
    }
    puts("Response received:");
    coap_show_pdu(pdu);
    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };

    /* prepare remote address */
    memcpy(&remote.addr.ipv6[12], SERVER_IP, sizeof(SERVER_IP));

    /* initialize the CoAP packet with a POST request */
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    const char *data = "Hello, COAP Cloud";
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    len += coap_payload(&pdu, (uint8_t *)data, strlen(data));

    /* send the request */
    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request");
    }

    return 0;
}