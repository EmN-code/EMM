#include "net/nanocoap/cache.h"
#include "gcoap.h"
#include "net/gcoap.h"
#include "stdio.h"
#include "string.h"

gcoap_request_memo_t memo;
coap_pkt_t pdu;
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    (void)memo;
    printf("Response received\n");
    if (pdu->payload_len) {
        printf("%.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}

int main(void) {
    printf("Starting CoAP client example\n");

    // Initialize remote server endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = CONFIG_GCOAP_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");

    // Initialize CoAP PDU
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");

    // Add payload to CoAP POST request
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send the CoAP request
    int res = gcoap_req_send(buf, pdu.payload_len + coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE), 
                             &remote, _resp_handler, &memo, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Error sending request\n");
    }

    return 0;
}