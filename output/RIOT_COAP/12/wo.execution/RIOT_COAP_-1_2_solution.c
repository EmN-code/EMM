#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <arpa/inet.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define PORT 5683

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, sock_udp_ep_t *remote) {
    (void)pdu;
    (void)remote;
    if (req_state == GCOAP_MEMO_ACK) {
        printf("Request acknowledged successfully\n");
    } else {
        printf("Request failed\n");
    }
}

int main(void) {
    uint8_t buf[128];
    sock_udp_ep_t remote = { .family = AF_INET, .port = PORT };
    coap_pkt_t pdu;
    ssize_t bytes;

    // Convert IP string to byte-represented form
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    // Initialize CoAP packet with POST method
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_code(&pdu, COAP_CODE_POST);

    // Set payload
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    // Send the CoAP request
    bytes = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes <= 0) {
        printf("Failed to send request\n");
        return -1;
    }

    printf("CoAP POST request sent\n");

    return 0;
}
