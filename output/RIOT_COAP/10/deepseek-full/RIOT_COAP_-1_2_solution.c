
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
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char *resp_buf = (char *)pdu->payload;
        printf("Response: %s\n", resp_buf);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the remote endpoint with the server IP
    if (sock_udp_str2ep(&remote, SERVER_IP) < 0) {
        puts("Error parsing server IP address");
        return 1;
    }

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_pkt_set_payload(&pdu, (uint8_t *)MESSAGE, sizeof(MESSAGE) - 1);

    // Send the CoAP request
    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
