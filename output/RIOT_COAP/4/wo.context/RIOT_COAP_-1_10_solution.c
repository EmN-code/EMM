#include <string.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT CONFIG_GCOAP_PORT
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define COAP_METHOD COAP_METHOD_POST


gcoap_resp_handler_t _resp_handler;

static void response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Response handler implementation */
    if (pdu != NULL) {
        printf("Received response: %d\n", coap_get_code_class(pdu));
    }
    else {
        printf("Error in response\n");
    }
}

int main(void) {
    sock_udp_ep_t remote;
    memset(&remote, 0, sizeof(remote));
    
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.port = COAP_SERVER_PORT;

    uint8_t buffer[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    int len = gcoap_req_init(&pdu, buffer, sizeof(buffer), COAP_METHOD, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    if (len != -1) {
        gcoap_req_send(buffer, len, &remote, response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    } else {
        printf("Error initializing request\n");
    }
    return 0;
}