// Necessary includes listed in descending order
#include <time_units.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <esp_system.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mutex.h>
#include <sys/socket.h>

// IP address and port constants
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    puts("Response received");
}

int main(void) {
    // Buffer for PDU
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    // Endpoint structure for the server
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address from string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error converting IP address");
        return -1;
    }

    // Initialize CoAP PDU structure
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Add payload to PDU
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE) - 1);
    pdu.payload_len = sizeof(MESSAGE) - 1;
    len += pdu.payload_len;

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error sending request");
        return -1;
    }

    puts("Request sent successfully");
    return 0;
}