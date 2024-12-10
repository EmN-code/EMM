
#include <stdio.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <esp_wifi.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        printf("Received response with code %u\n", coap_get_code(pdu));
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Initialize WiFi
    esp_wifi_init();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    // Convert IP address string to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Invalid IP address");
        return 1;
    }

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_add_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Send CoAP request
    ssize_t len = coap_get_total_pkt_len(&pdu);
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
