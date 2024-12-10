
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
    else {
        char *resp_phrase = gcoap_response_phrase(coap_get_code(pdu));
        printf("Response received: %s\n", resp_phrase);
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    if (sock_udp_name2ep(&remote, SERVER_IP, NULL) < 0) {
        puts("Error parsing server address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_pkt_write_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE));

    gcoap_req_send(buf, coap_pkt_get_total_bytes(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
