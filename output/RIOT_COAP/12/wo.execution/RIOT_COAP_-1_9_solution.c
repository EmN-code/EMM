#include <timex.h>
#include <xtimer.h>
#include <msg.h>
#include <mutex.h>      // For mutex control
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <net/sock/util.h> // For utility functions for sockets
#include <netinet/in.h>    // For sockaddr_in
#include <net/gcoap.h>     // For gcoap functions
#include <ztimer.h>      
#include <stdatomic.h>

// Define a buffer size for CoAP PDU
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

// CoAP server details
#define SERVER_IP        "47.102.103.1"
#define SERVER_PORT      5683
#define EMQX_URI_PATH    "/coap/test"

// Function to convert string IP to a byte-represented IP
int inet_pton(int af, const char *src, void *dst);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Response handler logic */
}

int main(void) {
    // Variable Declarations
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .port = SERVER_PORT };

    // converting IPv4 string to network format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid server IPv4 address\n");
        return -1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Fill payload with message
    size_t len = strlen("Hello, COAP Cloud");
    memcpy(pdu.payload, "Hello, COAP Cloud", len);
    pdu.payload_len = len;

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("CoAP POST request sent successfully\n");

    // Simulated event processing loop
    while (1) { }

    return 0;
}