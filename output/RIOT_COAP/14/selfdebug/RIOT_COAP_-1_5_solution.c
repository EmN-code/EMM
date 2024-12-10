#include<string.h>
#include<netinet/in.h>
#include<net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

ssize_t gcoap_req_send(const uint8_t *buf, size_t len,
                       const sock_udp_ep_t *remote,
                       gcoap_resp_handler_t resp_handler, void *context,
                       gcoap_socket_type_t tl_type);
                        
int gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len,
                   unsigned code, const char *path);

int inet_pton(int af, const char *src, void *dst);

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // handle response
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t result;
    
    // Convert the server IP address string to a binary format
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.netif  = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    
    // Initiate a CoAP PDU for a POST request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    
    // Add the message payload
    strcpy((char *)pdu.payload, "Hello, COAP Cloud");
    pdu.payload_len = strlen("Hello, COAP Cloud");
    
    // Send the CoAP request
    result = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    // Convert ssize_t to long int for correct format specifier in printf
    printf("Sent request, result: %ld\n", (long int)result);

    return 0;
}