#include <string.h>
#include <net/ipv4/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t len;
    
    // Initialize CoAP packet
    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        return -1;
    }
    
    // Set payload for the POST request
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    
    // Convert server IP address to binary form
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = CONFIG_GCOAP_PORT;  // Set to 5683
    
    // Send request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response
    (void) memo;
    (void) pdu;
    (void) remote;
}