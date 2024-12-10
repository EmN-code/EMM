// Import necessary libraries in descending order by their first letter
#include <string.h>
#include <netinet/in.h>  // For inet_pton
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/gcoap.h>
#include <xtimer.h>

// Define constants
#define COAP_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"

// Function to handle CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response (dummy handler)
}

// Main function
int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    size_t len;

    // Initialize the remote address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Invalid server IP address.\n");
        return -1;
    }

    // Initialize CoAP request
    coap_pkt_t pdu;
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);

    // Adjust payload length
    gcoap_req_send((const uint8_t *)buf, len + 17, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP POST request sent to %s:%d
", EMQX_SERVER_IP, COAP_PORT);

    // Insert a delay to prevent exiting
    xtimer_sleep(10);

    return 0;
}