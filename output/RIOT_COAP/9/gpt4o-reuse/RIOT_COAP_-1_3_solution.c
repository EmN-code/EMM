
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include "fmt.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (pdu->payload_len > 0) {
        printf("Received response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload_pos);
    } else {
        puts("No payload in response");
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    /* Set up the remote endpoint */
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error parsing server IP address");
        return 1;
    }

    /* Initialize the CoAP request */
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }

    /* Add payload to the request */
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    /* Send the request */
    gcoap_req_send(&buf[0], result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Run the event loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
