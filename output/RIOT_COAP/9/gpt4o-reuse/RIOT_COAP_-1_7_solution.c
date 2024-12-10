
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *resp_data = (char *)pdu->payload;
    size_t resp_len = pdu->payload_len;

    printf("Received response: %.*s\n", (int)resp_len, resp_data);
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = SERVER_PORT;

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv6[0], SERVER_IP) == NULL) {
        puts("Error: unable to parse server IP address");
        return 1;
    }

    remote.family = AF_INET;

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(&buf[0], &buf[sizeof(buf)], COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&buf[0], COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&buf[0], COAP_OPT_FINISH_PAYLOAD);

    memcpy(coap_get_payload(&buf[0]), "Hello, COAP Cloud", 17);
    len += 17;

    gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL);

    return 0;
}
