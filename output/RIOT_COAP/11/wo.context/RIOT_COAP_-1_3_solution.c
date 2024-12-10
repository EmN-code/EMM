#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>

#define SERVER_IP      "47.102.103.1"
#define SERVER_PORT    5683
#define URI_PATH       "/coap/test"
#define MESSAGE        "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6);
    remote.port = SERVER_PORT;

    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    len += strlen(MESSAGE);
    pdu.hdr_len = 0;  // Initialize hdr_len to avoid compilation errors
   
    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}