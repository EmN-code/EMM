
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "fmt.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *resp_buf = (char *)pdu->payload;
    size_t resp_len = pdu->payload_len;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        printf("Received response: %.*s\n", (int)resp_len, resp_buf);
    } else {
        printf("Error in response\n");
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_t server_addr;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (ipv4_addr_from_str(&server_addr, SERVER_IP) == NULL) {
        puts("Error: unable to parse destination address");
        return 1;
    }
    remote.addr.ipv4 = server_addr;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}