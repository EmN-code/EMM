// Include necessary headers in alphabetical order
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock.h>


// Define constants
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

// Define handler function
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote);

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    // Set remote endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif  = SOCK_ADDR_ANY_NETIF;
    remote.port = EMQX_SERVER_PORT;
    
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if(result != 1) {
        // Handle error in IP conversion
        return -1;
    }

    // Initialize CoAP packet
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send CoAP request
    ssize_t res = gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        // Handle error in sending request
        return -1;
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response
    if (pdu == NULL) {
        // No response received
        return;
    }

    // Process response here
}