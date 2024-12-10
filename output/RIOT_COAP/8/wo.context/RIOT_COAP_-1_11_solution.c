#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    // Configure the remote CoAP server endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr);
    
    // Prepare CoAP PDU storage and set up POST request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    
    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    
    // Set payload
    const char *PAYLOAD = "Hello, COAP Cloud";
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    
    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res > 0) {
        printf("Request sent successfully\n");
    } else {
        printf("Failed to send request\n");
    }

    return 0;
}