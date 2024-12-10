#include <xtimer.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <stdio.h>

// Define server endpoint and port
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT (5683)
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define RETRY_SLEEP (1U) // 1 second retry

static ssize_t _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, const sock_udp_ep_t *remote) {
    // A simple response handler for CoAP responses
    printf("Received response from the server\n");
    return len;
}

int main(void) {
    printf("Starting ESP32 CoAP client application\n");

    // Declare and initialize endpoint
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_SERVER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, COAP_SERVER_IP);

    // Setup CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    ssize_t bytes;  // To store result state of gcoap_req_send
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_payload(&pdu, (uint8_t *)COAP_MSG, sizeof(COAP_MSG) - 1);

    // Retry mechanism
    while (1) {
        // Send request
        bytes = gcoap_req_send(buf, pdu.total_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        // Check if sending is successful
        if (bytes <= 0) {
            printf("Failed to send CoAP request. Retrying in %u seconds...\n", RETRY_SLEEP);
            xtimer_sleep(RETRY_SLEEP);
        } else {
            printf("Successfully sent CoAP request\n");
            break;
        }
    }

    return 0;
}