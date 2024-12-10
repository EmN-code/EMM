// RIOT ESP32 CoAP POST Client Example

#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define CONFIG_GCOAP_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;  // Suppress unused variable warning
    if (pdu) {
        printf("Response received with code %u\n", coap_get_code(pdu));
    }
    else {
        printf("No response from server\n");
    }
}

int main(void) {
    coap_pkt_t pdu;
    sock_udp_ep_t remote;  
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = CONFIG_GCOAP_PORT;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: unable to convert IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    size_t len = gcoap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send request\n");
        return 1;
    }

    printf("Request sent successfully\n");
    return 0;
}