#include "thread_config.h"
#include "net/nanocoap/cache.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"

// Function to send CoAP POST request
static void send_coap_post(void) {
    // Buffer for CoAP message
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the CoAP packet with POST method and target URI
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    size_t len = strlen("Hello, COAP Cloud") + coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Add payload
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");

    printf("Sending CoAP POST request to COAP server\n");
    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    // Initialize the CoAP library
    gcoap_init();

    // Send a CoAP POST request
    send_coap_post();

    return 0;
}