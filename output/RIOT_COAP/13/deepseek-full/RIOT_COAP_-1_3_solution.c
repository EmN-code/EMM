
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    } else {
        printf("Response Code: %02x\n", coap_get_code(pdu));
    }
}

int main(void)
{
    esp_err_t ret = esp_wifi_start();
    if (ret != ESP_OK) {
        printf("WiFi start failed: %d\n", ret);
        return -1;
    }

    xtimer_sleep(2); // Allow some time for WiFi to connect

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_aton(SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    if (len < 0) {
        puts("Failed to prepare CoAP request");
        return -1;
    }

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
