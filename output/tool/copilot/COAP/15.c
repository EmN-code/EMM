#include <stdio.h>
#include <string.h>
#include "net/nanocoap_sock.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = COAP_SERVER_PORT
};

static void send_coap_post(void)
{
    uint8_t buf[128];
    uint8_t token[2] = {0x00, 0x01};
    coap_pkt_t pdu;
    ssize_t bytes;

    /* Set the remote address */
    ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, COAP_SERVER_ADDR);

    /* Initialize the CoAP packet */
    coap_pkt_init(&pdu, buf, sizeof(buf), 1, COAP_METHOD_POST, token, sizeof(token));
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_add_string(&pdu, COAP_OPT_URI_PATH, COAP_URI_PATH, '/');
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_payload(&pdu, (uint8_t *)COAP_PAYLOAD, strlen(COAP_PAYLOAD));

    /* Send the CoAP packet */
    bytes = nanocoap_request(&pdu, &remote, buf, sizeof(buf));
    if (bytes < 0) {
        printf("Error sending CoAP request: %d\n", (int)bytes);
    } else {
        printf("CoAP request sent successfully\n");
    }
}

int main(void)
{
    printf("RIOT CoAP client example\n");

    /* Send the CoAP POST request */
    send_coap_post();

    return 0;
}