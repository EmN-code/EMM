#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response from server
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    const char *EMQX_SERVER_IP = "47.102.103.1";
    const char *EMQX_URI_PATH = "/coap/test";
    const char *PAYLOAD = "Hello, COAP Cloud";

    // Set remote endpoint IP and port
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = 5683;
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Error setting IP address");
        return -1;
    }

    // Initialize the CoAP PDU
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send the request
    ssize_t len = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        puts("Failed to send request");
        return -1;
    }

    puts("Request sent successfully");
    return 0;
}