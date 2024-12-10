#include <stdio.h>
#include <string.h>
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static nanocoap_sock_t coap_sock;

int main(void)
{
    // Initialize the CoAP socket
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .port = SERVER_PORT,
    };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR);

    // Create the CoAP socket
    if (nanocoap_sock_create(&coap_sock, &remote) < 0) {
        puts("Error: unable to create CoAP socket");
        return 1;
    }

    // Prepare the CoAP POST request
    uint8_t buf[CONFIG_NANOCOAP_BLOCK_HEADER_MAX];
    coap_pkt_t pkt;
    pkt.hdr = (coap_hdr_t *)buf;
    pkt.payload = (uint8_t *)PAYLOAD;
    pkt.payload_len = strlen(PAYLOAD);

    // Build the CoAP header
    size_t len = coap_build_hdr(pkt.hdr, COAP_TYPE_CON, NULL, 0, COAP_METHOD_POST, nanocoap_sock_next_msg_id(&coap_sock));
    len += coap_opt_put_uri_pathquery(buf + len, NULL, URI_PATH);
    len += coap_opt_finish(&pkt, COAP_OPT_FINISH_PAYLOAD);

    // Send the CoAP POST request
    if (nanocoap_sock_send(&coap_sock, buf, len) < 0) {
        puts("Error: unable to send CoAP request");
        return 1;
    }

    puts("CoAP POST request sent successfully");

    // Close the CoAP socket
    nanocoap_sock_close(&coap_sock);

    return 0;
}