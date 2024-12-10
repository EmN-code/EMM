
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"
#include "shell.h"
#include "fmt.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    char *resp_buf = (char *)pdu->payload;
    size_t resp_len = pdu->payload_len;

    if (pdu->hdr->code == COAP_CODE_CONTENT) {
        printf("Received response: %.*s\n", (int)resp_len, resp_buf);
    } else {
        printf("No response received\n");
    }
}

static int _send_coap_request(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error parsing server IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

int main(void)
{
    puts("CoAP client example");

    /* Initialize the gcoap thread */
    gcoap_init();

    /* Send the CoAP request */
    _send_coap_request();

    /* Keep the thread alive to receive the response */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
