#include <gcoap.h>
#include <stdio.h>

// Define CoAP server parameters
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

// Function to initiate and send a CoAP request
static void send_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Set the remote server IP and port
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)(&remote.addr.ipv6), SERVER_IP);
    remote.port = SERVER_PORT;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Set the payload
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    // Set Content-Format option if needed (e.g., text/plain)
    // coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);

    // Send the CoAP request
    if (gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error: Unable to send CoAP request\n");
    }
}

// Response handler function
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    (void)memo;
    printf("Response received: %.*s\n", pdu->payload_len, (char *)pdu->payload);
}

int main(void) {
    puts("Starting CoAP client");
    send_coap_request();
    while (1) {
        // Main loop to keep the program running
    }
    return 0;
}