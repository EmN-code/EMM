
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/sock/udp.h"

/*
 * The response handler
 */
static void message_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    unsigned char *payload = pdu->payload;
    size_t payload_len = pdu->payload_len;

    printf("Received response: %.*s\n", (int)payload_len, payload);
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    /* Set the remote IP address */
    result = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    if (result != 1) {
        puts("Error setting remote IP address");
        return 1;
    }

    /* Initialize the CoAP request */
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (result < 0) {
        puts("Error initializing CoAP request");
        return 1;
    }

    /* Add payload to the request */
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    /* Send the request */
    gcoap_req_send(buf, result, &remote, message_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    /* Run the RIOT main loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
