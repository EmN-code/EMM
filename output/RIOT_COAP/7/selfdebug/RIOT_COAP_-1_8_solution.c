#include <string.h>  // For functions like memcpy
#include <net/ipv6/addr.h>  // For IPv6 address conversions
#include <net/gcoap.h>  // For gcoap related functions

#define COAP_METHOD_POST 0x02
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

const char EMQX_SERVER_IP[] = "47.102.103.1";
const char EMQX_URI_PATH[] = "/coap/test";
const char PAYLOAD[] = "Hello, COAP Cloud";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the CoAP response here
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Convert IP address
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Copy payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    gcoap_req_send(buf, pdu.payload - buf + strlen(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
