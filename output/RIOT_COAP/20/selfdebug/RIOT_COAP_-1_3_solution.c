#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h> // Ensure correct UDP socket functions are included
#include <net/coap.h> // Ensure correct CoAP functions are included

#define COAP_BUF_SIZE 128
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    // Handle server response (if needed)
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t server = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[COAP_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;

    // Convert server IP address
    int res = inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);
    if (res <= 0) {
        printf("Error converting IP address\n");
        return -1;
    }

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len <= 0) {
        printf("Error initializing CoAP request\n");
        return -1;
    }

    // Add payload - sending "Hello, COAP Cloud"
    const char *payload = "Hello, COAP Cloud";
    memcpy((char *)pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &server, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    printf("CoAP request sent\n");

    return 0;
}