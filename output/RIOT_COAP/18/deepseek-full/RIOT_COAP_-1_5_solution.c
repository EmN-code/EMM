
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Request timed out\n");
    } else if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    } else {
        printf("Error in response\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    // Initialize WiFi
    esp_wifi_init();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    // Convert IP address
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return -1;
    }

    // Initialize gcoap
    gcoap_init();

    // Prepare CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send CoAP request
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Wait for response
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
