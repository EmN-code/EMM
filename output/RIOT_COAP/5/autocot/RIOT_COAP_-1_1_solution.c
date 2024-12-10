#include <net/nanocoap/cache.h>
#include <gcoap.h>
#include <net/gnrc/netif.h>
#include <stdio.h>
#include <string.h>

void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handle the response (if needed) */
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };

    /* Parse IP address of server */
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, "47.102.103.1") == NULL) {
        puts("Error: unable to parse server IPv6 address");
        return 1;
    }
    memcpy(&remote.addr.ipv6, &addr, sizeof(addr));

    /* Initialize the CoAP packet */
    if (gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test") == -1) {
        puts("Error: unable to initialize CoAP request");
        return 1;
    }

    /* Add payload "Hello, COAP Cloud" */
    size_t payload_len = strlen("Hello, COAP Cloud");
    memcpy(pdu.payload, "Hello, COAP Cloud", payload_len);
    pdu.payload_len = payload_len;

    /* Send the CoAP request */
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error: unable to send CoAP request");
        return 1;
    }
    puts("CoAP request sent successfully!");
    return 0;
}