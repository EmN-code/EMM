
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char *class_str = (coap_pkt_class(pdu) == COAP_CLASS_SUCCESS) ? "Success" : "Error";
        printf("Response Code: %s\n", class_str);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_IP) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    while (1) {
        gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
        size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
        memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
        len += strlen(MESSAGE);

        gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        xtimer_sleep(5); // Send request every 5 seconds
    }

    return 0;
}
