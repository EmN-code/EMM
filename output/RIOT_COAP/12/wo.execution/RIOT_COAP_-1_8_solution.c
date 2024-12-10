#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Define the buffer size
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static sock_udp_ep_t coap_server;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the CoAP server address
    coap_server.family = AF_INET;
    coap_server.netif = SOCK_ADDR_ANY_NETIF;
    coap_server.port = COAP_SERVER_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &coap_server.addr.ipv4);

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send CoAP request
    if (gcoap_req_send(buf, pdu.payload_len + (pdu.payload - buf), &coap_server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) > 0) {
        printf("Request sent to %s\n", COAP_SERVER_IP);
    } else {
        printf("Error sending request\n");
    }

    // Let it run indefinitely
    while (1) {
        xtimer_sleep(5);
    }
    return 0;
}