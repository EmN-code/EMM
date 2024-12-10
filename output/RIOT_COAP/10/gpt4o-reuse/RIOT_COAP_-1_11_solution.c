
#include "gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char buf[32];
    if (coap_get_payload(pdu, (const uint8_t **)&buf)) {
        printf("Received: %s\n", buf);
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    /* Set the remote endpoint */
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    /* Set the payload */
    const char *payload = "Hello, COAP Cloud";
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, payload, strlen(payload));

    /* Send the request */
    ssize_t bytes_sent = gcoap_req_send(&pdu, pdu.payload - pdu.hdr, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent < 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    /* Run the event loop */
    while (1) {
        gcoap_event_loop();
        xtimer_sleep(1);
    }

    return 0;
}
