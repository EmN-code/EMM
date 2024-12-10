
#include <stdio.h>
#include <string.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static uint8_t buf[BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    char *payload = (char *)coap_get_payload(pdu);
    if (payload) {
        printf("Received CoAP response: %s\n", payload);
    } else {
        printf("No payload in CoAP response\n");
    }
}

int main(void)
{
    coap_pkt_t pdu;
    int result;

    /* Initialize remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        puts("Error converting IP address");
        return 1;
    }

    /* Initialize CoAP request */
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    /* Set payload */
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    /* Send CoAP request */
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Main loop */
    while (1) {
        /* Keep the thread alive */
        xtimer_sleep(1);
    }

    return 0;
}
