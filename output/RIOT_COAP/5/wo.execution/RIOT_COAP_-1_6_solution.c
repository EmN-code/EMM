#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 256
#define GCOAP_MEMO_TIMEOUT           (30 * MS_PER_SEC)

static const char *COAP_SERVER_IP = "47.102.103.1";
static const char *COAP_URI_PATH = "/coap/test";
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static coap_pkt_t pdu;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error in IP address conversion\n");
        return -1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        printf("Error in CoAP request initialization\n");
        return -1;
    }

    const char *payload = "Hello, COAP Cloud";
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    if (len < 0) {
        printf("Error finishing options\n");
        return -1;
    }

    len += coap_opt_add(&pdu, COAP_OPT_CONTENT_FORMAT, COAP_FORMAT_TEXT, payload, strlen(payload));
    if (len < 0) {
        printf("Error adding payload\n");
        return -1;
    }

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return -1;
    }

    puts("CoAP request sent successfully\n");
    return 0;
}