#include <stdio.h>
#include <stdint.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Response handler function
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu == NULL) {
        puts("Error receiving response");
    } else {
        puts("Received response");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netmask_len = 0;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);
    remote.port = COAP_SERVER_PORT;

    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    memcpy(pdu.payload, COAP_PAYLOAD, sizeof(COAP_PAYLOAD));
    pdu.payload_len = sizeof(COAP_PAYLOAD) - 1;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}