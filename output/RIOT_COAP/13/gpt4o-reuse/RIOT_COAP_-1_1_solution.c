
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "shell.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        char *payload = (char *)pdu->payload;
        printf("Received response: %s\n", payload);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    remote.family = AF_INET;

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    const char *message = "Hello, COAP Cloud";
    size_t len = strlen(message);
    memcpy(pdu.payload, message, len);
    pdu.payload_len = len;

    gcoap_req_send(&buf[0], pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}