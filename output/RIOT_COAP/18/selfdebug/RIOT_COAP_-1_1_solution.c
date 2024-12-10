#include "net/nanocoap/cache.h"
#include "gcoap.h"
#include <stdio.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

/* Function to initialize and send a CoAP POST request */
void send_coap_post(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Parse the string address to IP endpoint
    char addr[] = SERVER_ADDR;
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, addr) == NULL) {
        puts("Error: unable to parse server address");
        return;
    }

    // Initialize the CoAP packet
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) < 0) {
        puts("Error: unable to initialize CoAP request");
        return;
    }

    // Add the payload to the CoAP request
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    gcoap_finish(&pdu, strlen(MESSAGE), COAP_OPT_FINISH_PAYLOAD);

    // Send the request and handle the response
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error: unable to send CoAP request");
    }
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received");
}

int main(void) {
    puts("Starting RIOT CoAP POST client\n");
    send_coap_post();
    return 0;
}