
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
        char *class_str = (pdu->hdr->code < 32) ? "Request" : "Response";
        printf("CoAP %s, Code %1u.%02u", class_str, pdu->hdr->code >> 5, pdu->hdr->code & 0x1F);
        if (gcoap_resp_code_to_str(pdu->hdr->code) != NULL) {
            printf(" (%s)\n", gcoap_resp_code_to_str(pdu->hdr->code));
        }
        else {
            puts("");
        }
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    while (1) {
        gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
        ssize_t payload_len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
        memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
        payload_len += strlen(MESSAGE);

        gcoap_req_send(buf, payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        xtimer_sleep(5);  // Wait for 5 seconds before sending the next request
    }

    return 0;
}
