#include <stdint.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "xtimer.h"

#define ENABLE_DEBUG (1)
#include "debug.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (pdu != NULL) {
        DEBUG("Response received:\n");
        DEBUG("Payload: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    }
    else {
        DEBUG("No response received\n");
    }
}

int main(void) {
    msg_init_queue(NULL, 0);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Initialize the CoAP packet */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    /* Payload "Hello, COAP Cloud" */
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    /* Establish server endpoint */
    sock_udp_ep_t server = {
        .family = AF_INET,
        .port = COAP_SERVER_PORT,
    };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr.ipv6, COAP_SERVER_IP);

    /* Send the request */
    ssize_t res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        DEBUG("Error sending packet\n");
    }

    DEBUG("Sent CoAP POST request\n");

    /* Wait so we can receive the response */
    xtimer_sleep(5);

    return 0;
}