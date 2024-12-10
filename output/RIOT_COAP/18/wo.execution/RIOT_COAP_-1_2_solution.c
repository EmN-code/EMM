#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <esp_system.h>

#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Implement your response handling logic here
}

int main(void) {
    // Prepare CoAP URI and buffer
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
     
    // Initialize CoAP request with POST method and specified URI
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        // Handle error during initialization
    }
    
    // Set payload of the request
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    gcoap_finish(&pdu, strlen(payload), COAP_FORMAT_TEXT);

    // Configure remote endpoint
    sock_udp_ep_t remote = {
        .family = AF_INET,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = 5683
    };
    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);

    // Send the CoAP request
    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        // Handle sending error
    }

    return 0;
}