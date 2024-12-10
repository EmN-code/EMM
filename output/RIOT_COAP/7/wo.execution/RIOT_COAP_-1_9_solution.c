#include "xtimer.h"
#include "stdio.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"

// Define the server IP address and port
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

// Function to convert IP address
int _inet_pton(int af, const char *src, void *dst) {
    return inet_pton(af, src, dst);
}

// Response handler
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    printf("Response received: %s\n", (char *)pdu);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;
    sock_udp_ep_t remote;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    
    // Convert IP address
    result = _inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: invalid IP address\n");
        return -1;
    }
    remote.port = SERVER_PORT;
    
    // Prepare payload
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send the request
    result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (result < 0) {
        printf("Error sending request\n");
        return -1;
    }

    // Wait here for a response (blocking operation)
    xtimer_sleep(5);

    return 0;
}