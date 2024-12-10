// Description of the problem and approach
// The task is to send a CoAP POST request from an ESP32 using RIOT-OS to a specific server address.
// We will use the `gcoap_req_init` and `gcoap_req_send` functions to initialize and send the request.

#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/gnrc.h"
#include "msg.h"
#include <stdio.h>
#include <string.h>

#define COAP_SERVER_ADDR     "47.102.103.1"  // CoAP Server IP
#define COAP_SERVER_PORT     CONFIG_GCOAP_PORT // Using defined port 5683
#define COAP_PATH            "/coap/test"
#define COAP_METHOD          COAP_METHOD_POST
#define COAP_REQ_PAYLOAD     "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                           const sock_udp_ep_t *remote)
{
    (void)remote; // Silence compiler warning
    if (pdu->hdr->code != COAP_CODE_EMPTY) {
        printf("Response code: %u.%u\n", COAP_CODE_CLASS(pdu->hdr->code), 
                COAP_CODE_DETAIL(pdu->hdr->code));
    }
    else {
        puts("Request sent successfully, received empty ACK");
    }
}

int main(void)
{
    // Prepare the CoAP endpoint
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET6;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_ADDR);

    // Buffer for the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize request packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD, COAP_PATH);
    memcpy(pdu.payload, COAP_REQ_PAYLOAD, sizeof(COAP_REQ_PAYLOAD));
    size_t len = gcoap_finish_payload(&pdu, sizeof(COAP_REQ_PAYLOAD) - 1);

    // Send the CoAP POST request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending the request");
    }

    // Wait for response
    char stack[THREAD_STACKSIZE_MAIN];
    msg_init_queue((msg_t *)stack, THREAD_STACKSIZE_MAIN);
    msg_t msg;
    while (msg_receive(&msg)) {
        // Handle messages
    }

    return 0;
}