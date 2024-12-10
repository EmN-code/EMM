#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr, SERVER_ADDR);

    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t len;

    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_uint(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    if (sock_udp_send(NULL, buf, pdu.payload_len, &remote) < 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP POST request sent to %s:%d\n", SERVER_ADDR, SERVER_PORT);
    return 0;
}