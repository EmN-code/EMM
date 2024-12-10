
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "fmt.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len = sizeof(buf);
    gcoap_response(pdu, buf, len, COAP_CODE_CONTENT);

    printf("CoAP response: ");
    for (size_t i = 0; i < pdu->payload_len; i++) {
        printf("%c", pdu->payload[i]);
    }
    printf("\n");
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Add payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    // Send the request
    ssize_t sent = gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (sent < 0) {
        printf("Error sending CoAP request: %d\n", (int)sent);
        return 1;
    }

    // Run the RIOT main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
