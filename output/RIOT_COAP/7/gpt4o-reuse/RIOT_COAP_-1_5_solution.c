
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (pdu) {
        printf("Received CoAP response\n");
    } else {
        printf("No response received\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    int result;

    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid server IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, payload, payload_len);

    gcoap_req_send(buf, coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    while (1) {
        // Main loop to keep the application running
    }

    return 0;
}
