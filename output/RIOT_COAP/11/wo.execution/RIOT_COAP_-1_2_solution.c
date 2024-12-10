#include "mma8x5x_regs.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/netif.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define GCOAP_SOCKET_TYPE_UDP SOCK_ADDR_TYPE_UDP

static const char *EMQX_URI_PATH = "/coap/test";
static const char *COAP_MESSAGE = "Hello, COAP Cloud";

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    /* Initialize the remote endpoint */
    netif_t *netif = netif_get_by_name("esp0");
    if (!netif) {
        puts("Error: No interface named 'esp0'");
        return -1;
    }
    remote.netif = netif;
    remote.family = AF_INET6;  // Use IPv6 for the scope of this exercise
    remote.port = 5683;
    
    /* Convert string IP to binary, replacing IPV4 usage */
    ipv6_addr_t addr;
    uint8_t ip[] = {47, 102, 103, 1};
    memcpy(addr.u8, ip, sizeof(ip)); 
    memcpy(&remote.addr.ipv6, &addr.u8, sizeof(ip));
    
    /* Initialize CoAP request */
    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len == 0) {
        puts("Error: unable to initialize request PDU");
        return -1;
    }

    /* Set the payload of the CoAP request */
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.hdr->payload_len = strlen(COAP_MESSAGE);

    /* Send CoAP request */
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error: unable to send request");
        return -1;
    }

    puts("CoAP request sent");
    return 0;
}

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        puts("Timeout for response");
        return;
    }

    if (pdu->payload_len) {
        printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
    } else {
        puts("Empty response received");
    }
}