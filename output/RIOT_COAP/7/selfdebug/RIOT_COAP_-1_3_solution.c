# include <stdio.h>
# include "net/ipv6/addr.h"
# include "net/sock/udp.h"
# include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu) {
        printf("Response received with code %u\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int result;
    
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);
    
    result = inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: Could not convert server address\n");
        return -1;
    }
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    
    if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
        return -1;
    }
    
    printf("Request sent\n");
    return 0;
}