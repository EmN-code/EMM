#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/conn/udp.h>

#include "gcoap.h"

#define COAP_URI_PATH "/coap/test"
#define COAP_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    gcoap_listener_t listener;
    gcoap_register_listener(&listener);

    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    
    // Initialize the remote endpoint with IPv4 address
    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address.");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = 5683;

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Set payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        puts("Error sending request.");
        return -1;
    }

    return 0;
}

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu == NULL) {
        puts("No response received.");
        return;
    }
    puts("Response received:");
    for (size_t i = 0; i < pdu->payload_len; i++) {
        printf("%c", pdu->payload[i]);
    }
    puts("");
}