#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/ipv6/addr.h>

#define COAP_PORT 5683
#define SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pkt, const sock_udp_ep_t *from)
{
    /* handle the response, e.g., print status, check payload, etc. */
    return 0;
}

int main(void)
{
    int result;
    sock_udp_ep_t remote;

    /* Prepare end point sockaddr structure */
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    /* Initialize the address structure using inet_pton */
    if ((result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4)) <= 0) {
        /* Handle error */
        return -1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Prepare CoAP message */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    /* Send CoAP request */
    ssize_t len = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        /* Handle error */
    }

    return 0;
}