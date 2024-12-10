#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

// Configuration macros
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define COAP_SERVER_IP "47.102.103.1"

// Define buffer for CoAP PDU
uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    // Handle CoAP response
    if (pdu) {
        printf("CoAP response received\n");
    }
    else {
        printf("Failed to receive CoAP response\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    ssize_t bytes;
    sock_udp_ep_t remote;

    // Initialize the request packet
    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (res < 0) {
        printf("Failed to initialize CoAP request: %d\n", res);
        return res;
    }

    // Set the payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Set remote endpoint (IPv4)
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = CONFIG_GCOAP_PORT;
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Send the CoAP request
    bytes = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes <= 0) {
        printf("Failed to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}