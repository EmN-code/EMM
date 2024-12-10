#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling logic
}

int main(void) {
    // Defining the buffer for the request
    uint8_t buf[128];

    // Initializing the CoAP packet
    coap_pkt_t pdu;

    // Set remote endpoint details
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");

    // Setting the payload
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Sending the CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        // Error sending request
    }
    return 0;
}