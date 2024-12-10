#include <net/ipv4/addr.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

gcoap_request_memo_t req_memo;

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Handle response
}

int main(void) {
    // Prepare remote endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP string to binary format
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP PDU
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);

    // Prepare payload
    const char *msg = "Hello, COAP Cloud";
    size_t msg_len = strlen(msg);
    memcpy(pdu.payload, msg, msg_len);
    pdu.payload_len = msg_len;

    // Call function to send request with necessary handlers
    gcoap_req_send(buf, msg_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}