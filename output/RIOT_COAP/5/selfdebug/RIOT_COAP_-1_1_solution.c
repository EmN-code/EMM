#include <string.h>
#include <stdio.h>
#include "net/gcoap.h"
#include "msg.h"
#include "xtimer.h"

// Define the message buffer for the main thread
#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_REQ_SENT) {
        puts("Request sent successfully");
    }
    else {
        puts("Failed to send request");
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // CoAP server address
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .port = CONFIG_GCOAP_PORT,
    };

    // Set the IPv6 address
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");

    // Define CoAP PDU
    gcoap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;

    // Initialize the CoAP request
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");

    // Payload
    const char *message = "Hello, COAP Cloud";
    memcpy(pdu.payload, message, strlen(message));
    len += strlen(message);

    // Send the request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        puts("CoAP request sent");
    }
    else {
        puts("CoAP request failed");
    }

    // Prevent main thread from exiting
    while (1) {
        xtimer_sleep(5);
    }

    return 0;
}