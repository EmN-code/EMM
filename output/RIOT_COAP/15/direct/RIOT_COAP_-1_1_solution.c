#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/coap.h"
#include "net/af.h"
#include "kernel.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

int main(void) {
    printf("RIOT CoAP client example\n");
    
    /* Create a UDP endpoint for CoAP communication */
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR);

    /* Prepare CoAP PDU */
    uint8_t buf[128];
    coap_pkt_t pdu;
    coap_pdu_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, coap_get_mid());
    coap_opt_add_uri_path(&pdu, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    coap_payload(&pdu, (uint8_t *)payload, strlen(payload));
    
    /* Send CoAP POST request */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error creating UDP socket\n");
        return 1;
    }

    if (sock_udp_send(&sock, pdu.hdr->data, pdu.hdr->len, &remote) < 0) {
        printf("Error sending UDP packet\n");
        return 1;
    }

    printf("CoAP POST request sent\n");

    /* Close socket */
    sock_udp_close(&sock);

    return 0;
}