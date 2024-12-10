#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_METHOD COAP_METHOD_POST
#define PAYLOAD "Hello, COAP Cloud"

// Buffer size for CoAP PDU
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;

    if (pdu != NULL) {
        printf("Response received: %s\n", (char *)pdu->payload);
    } else {
        printf("No response \n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;  // Correct family assignment
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    int res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD, URI_PATH);
    if (res < 0) {
        printf("Error initializing CoAP request\n");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t payload_len = strlen(PAYLOAD);

    ssize_t bytes = gcoap_req_send(&buf[0], res + payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes < 0) {
        printf("Error sending CoAP request. Code: %d\n", bytes);
        return 1;
    }

    printf("CoAP request sent\n");

    return 0;
}