#include <stdio.h>
#include "net/gcoap.h"
#include "msg.h"
#include "shell.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

// Buffer for the CoAP PDU
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

// Callback function for handling the response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %d\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

void send_coap_request(void) {
    sock_udp_ep_t remote;

    remote.family = AF_INET6;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    unsigned len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
}

int main(void) {
    // Initialize the gcoap module
    gcoap_init();

    // Send the CoAP request
    send_coap_request();

    // Wait for incoming messages/events
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}