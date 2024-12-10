
#include <stdio.h>
#include <string.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <netinet/in.h>
#include <evtimer_mbox.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <sock_udp_str2ep.h>
#include <coap_opt_add_format.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        printf("Response received: %u\n", (unsigned)pdu->payload_len);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = EMQX_SERVER_PORT;

    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    ssize_t len = gcoap_finish(&pdu, 0);
    if (len <= 0) {
        puts("Failed to create CoAP request");
        return 1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    puts("CoAP request sent");

    return 0;
}
