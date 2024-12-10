#include <stdio.h>  // for common input/output functions
#include <string.h>  // for string handling functions
#include "thread_config.h"  // RIOT thread configuration
#include "net/nanocoap.h"  // minimal CoAP library
#include "gnrc/netif.h"  // generic network interface
#include "net/gcoap.h"  // CoAP implementation for RIOT-OS

/* Define the URI path and server endpoint */
#define COAP_URI_PATH           "/coap/test"
#define COAP_SERVER_IP          "47.102.103.1"
#define COAP_SERVER_PORT        CONFIG_GCOAP_PORT

/* CoAP request buffer */
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

void send_coap_request(void) {
    /* CoAP PDU */
    coap_pkt_t pdu;
    /* Remote server */
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr);

    /* Initialize the CoAP request */
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);

    /* Add the payload */
    const char *payload = "Hello, COAP Cloud";
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    /* Send the request */
    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP message sent to server %s:%d\n", COAP_SERVER_IP, COAP_SERVER_PORT);
}

int main(void) {
    /* Call the function to send CoAP request */
    send_coap_request();

    return 0;
}