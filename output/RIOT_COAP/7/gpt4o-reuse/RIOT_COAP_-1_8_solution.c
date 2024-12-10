
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        char buf[CONFIG_GCOAP_PDU_BUF_SIZE];
        size_t len = gcoap_response(pdu, (uint8_t *)buf, sizeof(buf), 0);
        if (len > 0) {
            printf("Received response: %s\n", buf);
        }
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    // Initialize the CoAP request
    int res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (res < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    // Add payload to the request
    const char *payload = "Hello, COAP Cloud";
    gcoap_add_payload(&pdu, (uint8_t *)payload, strlen(payload));

    // Send the request
    gcoap_req_send(&buf[0], res, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Run the event loop to process the response
    while (1) {
        gcoap_event_loop();
    }

    return 0;
}
