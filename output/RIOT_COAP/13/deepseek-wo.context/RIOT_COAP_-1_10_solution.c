
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>  // Include this header for inet_pton
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUF_SIZE (CONFIG_GCOAP_PDU_BUF_SIZE)

static uint8_t buf[BUF_SIZE];
static sock_udp_ep_t remote;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    char *resp_buf = (char *)pdu->payload;
    size_t resp_len = pdu->payload_len;

    printf("Received CoAP response: %.*s\n", (int)resp_len, resp_buf);
}

int main(void)
{
    // Initialize the remote endpoint
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Set the payload
    const char *payload = "Hello, COAP Cloud";
    size_t payload_len = strlen(payload);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    // Finish the CoAP packet
    gcoap_finish(&pdu, payload_len, COAP_FORMAT_TEXT);

    // Send the CoAP request
    gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Sleep to allow the request to be sent and response to be received
    xtimer_sleep(2);

    return 0;
}
