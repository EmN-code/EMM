
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
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
    else {
        char *resp_phrase = (char *)gcoap_response_phrase(coap_get_code(pdu));
        printf("Received response: %s\n", resp_phrase);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Initialize the remote endpoint
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid server IP address");
        return 1;
    }

    // Initialize the CoAP request
    result = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (result < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    // Set the payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    result = coap_opt_add_format(&pdu, COAP_FORMAT_TEXT_PLAIN);
    if (result < 0) {
        puts("Failed to add format option");
        return 1;
    }
    result = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (result < 0) {
        puts("Failed to finish options");
        return 1;
    }
    memcpy(pdu.payload, payload, payload_len);

    // Send the CoAP request
    result = gcoap_req_send(buf, result, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
