#include <string.h>
#include <stdio.h>
#include <net/nanocoap.h> // Required for coap-related structures and functions
#include <net/ipv4/addr.h> // For inet_pton function

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    remote.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.port = SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));

    ssize_t bytes_sent = gcoap_req_send(buf, strlen(COAP_MESSAGE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent <= 0) {
        printf("Failed to send request\n");
    }
    
    return 0;
}