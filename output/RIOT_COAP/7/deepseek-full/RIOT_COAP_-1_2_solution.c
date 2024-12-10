
#include <netinet/in.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <esp_system.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        printf("Response Code: %u\n", coap_get_code(pdu));
    }
}

int main(void)
{
    esp_err_t ret = esp_wifi_start();
    if (ret != ESP_OK) {
        printf("WiFi start failed: %d\n", ret);
        return -1;
    }

    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Invalid server IP address");
        return -1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    ssize_t len = gcoap_request_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL);
    if (len < 0) {
        puts("Failed to send request");
        return -1;
    }

    puts("Request sent successfully");
    return 0;
}
