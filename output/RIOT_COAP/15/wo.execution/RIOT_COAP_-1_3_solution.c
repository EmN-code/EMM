#include "sys/socket.h"
#include "netinet/in.h"
#include "mutex.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include <string.h>
#include <stdio.h>

// Correctly configured PDU buffer size
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define PAYLOAD "Hello, COAP Cloud"
#define EMQX_URI_PATH "/coap/test"

// Server details
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683

static sock_udp_ep_t remote;
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static coap_pkt_t pdu;

// Response handler function
static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_FIN) {
        printf("Response received: %u\n", coap_get_code(pdu));
    }
}

int main(void) {
    // Initialize the remote server endpoint
    remote.family = AF_INET;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = EMQX_SERVER_PORT;

    // Initialize the CoAP request
    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        printf("Failed to initialize CoAP request\n");
        return 1;
    }

    // Copy the payload into the PDU payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the request over UDP
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }
    return 0;
}