#include <net/sock/udp.h>  // Include necessary network modules

// Configuration defines
#define GCOAP_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define SERVER_IP "47.102.103.1"

// Buffer size for CoAP PDU
#define PDU_BUF_SIZE 128
uint8_t buf[PDU_BUF_SIZE];

// CoAP request initialization and sending function
void send_coap_request(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    // Initialize the CoAP request
    int result = gcoap_req_init(&pdu, buf, PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (result < 0) {
        // Handle error
        return;
    }

    // Set payload
    coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 16);

    // Configure the remote server endpoint
    remote.family = AF_INET;
    remote.port = GCOAP_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr);

    // Send the CoAP request
    bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        // Handle error in sending
        return;
    }
}

int main(void) {
    // Create UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        // Handle error
        return 1;
    }

    // Send CoAP request
    send_coap_request();

    return 0;
}