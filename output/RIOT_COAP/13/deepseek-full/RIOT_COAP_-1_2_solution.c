
#include <stdio.h>
#include <string.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <ztimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        printf("Response Code: %02x\n", coap_get_code(pdu));
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t len;

    if (sock_udp_str2ep(&remote, SERVER_IP, SERVER_PORT) < 0) {
        puts("Error parsing server address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
