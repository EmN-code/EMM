#include <string.h>
#include <stdio.h>
#include <net/gcoap.h>
#include <gcoap.h>
#include <coap.h>
#include <net/gnrc.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"

static const char *uri_path = "/coap/test";

/* Predefined CoAP message buffer size */
#define _BUF_SIZE               (128U)

static uint8_t _buf[_BUF_SIZE];

static ssize_t _build_post(uint8_t *buf, size_t len) {
    coap_pkt_t pdu;
    ssize_t bytes;
    
    coap_pkt_init(&pdu, buf, len, COAP_METHOD_POST, COAP_TYPE_CON);  
    
    coap_opt_add_uri_path(&pdu, uri_path);
    
    const char *message = "Hello, COAP Cloud";
    bytes = coap_pkt_append_payload(&pdu, (uint8_t*)message, strlen(message));

    return (bytes < 0) ? bytes : coap_hdr_len(&pdu) + bytes;
}

static void _send_coap_request(void) {
    sock_udp_ep_t server = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&server.addr, SERVER_ADDR);
    server.port = atoi(SERVER_PORT);

    ssize_t bytes = _build_post(_buf, _BUF_SIZE);

    if (bytes <= 0) {
        printf("Error building CoAP message\n");
        return;
    }

    if (gcoap_req_send(_buf, bytes, &server, NULL, NULL) <= 0) {
        puts("gcoap_req_send() failed");
        return;
    }

    puts("CoAP POST request sent\n");
}

int main(void) {
    gcoap_register_listener(NULL);
    _send_coap_request();

    return 0;
}