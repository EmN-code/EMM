#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

// Define some essential parameters
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo == NULL) {
        puts("Error: Invalid memo on response");
        return -1;
    }
    printf("Response received with code %u\n", coap_get_code(pdu));
    return 0;
}

int main(void) {
    // Define buffer for CoAP PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Parse the IP address
    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        puts("Error: Unable to parse IP address");
        return -1;
    }

    // Set remaining parameters
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error: Failed to send CoAP request");
        return -1;
    }

    puts("CoAP request sent.");
    return 0;
}