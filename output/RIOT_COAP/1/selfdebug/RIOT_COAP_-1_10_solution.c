#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>  // Ensures inet_pton declaration

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_PORT 5683  // Correctly define COAP_PORT

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo; (void)remote;  // Suppress unused parameter warnings
    printf("Response Handler: received response of len: %u\n", pdu->payload_len);
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Convert string IP address to binary form; fix termination issues
    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        puts("ERROR: Unable to convert IP address.");
        return -1;
    }

    coap_pkt_t pdu;
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    if (len < 0) {
        puts("ERROR: Unable to initialize request.");
        return -1;
    }
    char *payload = "Hello, COAP Cloud";  // Define payload
    memcpy(pdu.payload, payload, strlen(payload));  // Properly terminate payload's character arrays
    len += strlen(payload);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("ERROR: Unable to send request.");
        return -1;
    }

    printf("CoAP request sent to 47.102.103.1:%d\n", COAP_PORT);
    return 0;
}