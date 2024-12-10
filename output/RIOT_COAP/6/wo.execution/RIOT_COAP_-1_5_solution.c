#include <unistd.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "net/gcoap.h"

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 64
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define GCOAP_SOCKET_TYPE_UDP 0

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void) memo;
    (void) pdu;
    (void) remote;
    // Handle response here
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t len;

    // Initialize PDU
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set payload
    const char *payload = "Hello, COAP Cloud";
    coap_pkt_set_payload(&pdu, buf, payload, strlen(payload));

    // Finish PDU
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Convert IP address
    remote.family = AF_INET;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;  // Set the port to the default CoAP port

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}