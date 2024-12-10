#include <netinet/in.h>  // To include the inet_pton function
#include <net/sock/udp.h> // For UDP socket functionality
#include <net/ipv4/addr.h> // For AF_INET

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static uint8_t buf[128];  // Buffer for the payload

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handler code
}

int main(void) {
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    
    // Convert the server IP address from text to binary form
    int result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        if (result == 0) {
            printf("inet_pton: Invalid address\n");
        } else {
            perror("inet_pton");
        }
        return -1;
    }

    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    ssize_t len = gcoap_req_send(buf, pdu.payload_len + (pdu.payload - (uint8_t *)&pdu), 
                                 &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (len <= 0) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    return 0;
}