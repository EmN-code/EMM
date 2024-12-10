#include <net/nanocoap.h>
#include <msg.h>
#include <gcoap.h>

#define COAP_SERVER_IP      "47.102.103.1"
#define COAP_SERVER_PORT    CONFIG_GCOAP_PORT  // 5683
#define URI_PATH            "/coap/test"
#define MESSAGE             "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote; // Suppress unused parameter warning
    if (memo->state == GCOAP_MEMO_REPLY) {
        printf("Response received: %d \n", pdu->payload_len);
    } else {
        puts("Request failed");
    }
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET6,
                             .port = COAP_SERVER_PORT };

    /* set the server IP address */
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    /* initialize the CoAP packet */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    /* copy payload into the packet */
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE));
    pdu.payload_len = sizeof(MESSAGE);

    /* send the CoAP packet */
    if (gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send request");
    }

    /* wait to keep the main function alive */
    while (1) {
        msg_receive(&msg);
    }

    return 0;
}