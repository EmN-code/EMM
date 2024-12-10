#include <stdio.h>
#include "thread_config.h"
#include "net/nanocoap/cache.h"

#include "gcoap.h"
#include "net/sock/udp.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

// UDP endpoint
static const sock_udp_ep_t server = {
    .family = AF_INET,
    .netif = SOCK_ADDR_ANY_NETIF,
    .port = COAP_SERVER_PORT
};

/* Response handler for CoAP responses */
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP_TRUNC) {
        puts("Error: response buffer too small");
    }
    else {
        printf("Response received with code %u\n", coap_get_code(pdu));
    }
}

int main(void)
{
    char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    uint8_t pdu_buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    /* Initialize CoAP packet */
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, pdu_buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len == 0) {
        puts("Error: unable to initialize CoAP request");
        return -1;
    }

    /* Set the payload to "Hello, COAP Cloud" */
    const char payload[] = "Hello, COAP Cloud";
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE) == 0) {
        printf("Error finishing options\n");
        return -1;
    }
    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    /* Send CoAP packet */
    if (gcoap_req_send(&pdu_buf[0], len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) >= 0) {
        printf("CoAP POST request sent to %s\n", COAP_SERVER_IP);
    }
    else {
        puts("Error sending CoAP POST request");
        return -1;
    }

    return 0;
}