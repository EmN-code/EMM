#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/coap.h"

#define SERVER_ADDR      "47.102.103.1"
#define SERVER_PORT      5683
#define URI_PATH         "/coap/test"
#define PAYLOAD          "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = SERVER_PORT
    };

    /* set server address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_ADDR) == NULL) {
        puts("Error: unable to parse server address");
        return 1;
    }

    /* prepare CoAP PDU */
    uint8_t buf[128];
    coap_pkt_t pdu;
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_TYPE_CON);
    coap_hdr_set_uri_path(&pdu, URI_PATH);
    coap_hdr_set_code(&pdu, COAP_CODE_POST);
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
    pdu.payload_len = sizeof(PAYLOAD);

    size_t len = coap_build(&pdu, buf, sizeof(buf));
    if (len == 0) {
        puts("Error: failed to build CoAP message");
        return 1;
    }

    /* send message */
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL) < 0) {
        puts("Error: unable to create UDP socket");
        return 1;
    }

    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        puts("Error: unable to send UDP message");
        sock_udp_close(&sock);
        return 1;
    }

    puts("CoAP message sent successfully");
    sock_udp_close(&sock);
    return 0;
}