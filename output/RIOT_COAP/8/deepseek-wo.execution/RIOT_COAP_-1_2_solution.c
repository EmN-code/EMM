
#include <net/ipv6/addr.h>
#include <netinet/in.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <sys/socket.h>
#include <evtimer_msg.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <net/sock/util.h>
#include <net/gcoap.h>

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
    else if (memo->state == GCOAP_MEMO_RESP) {
        char *resp_buf = (char *)pdu->hdr;
        size_t resp_len = pdu->payload_len;
        resp_buf[resp_len] = '\0';
        puts(resp_buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert IP address string to binary form
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_add_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send CoAP request
    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
