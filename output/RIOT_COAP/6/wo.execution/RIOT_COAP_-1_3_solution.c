#include "thread_config.h"
#include "thread.h"
#include "sys/socket.h"
#include "sock/udp.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "nanocoap/cache.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "event/thread.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received with code: %d\n", coap_get_code(pdu));
    } else {
        printf("No response for the sent request.\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid server IP address\n");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len == 0) {
        printf("Failed to initialize CoAP request\n");
        return -1;
    }

    coap_hdr_set_type(&pdu, COAP_TYPE_CON);
    coap_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send(buf, len, &remote, _response_handler, NULL) == -1) {
        printf("Failed to send CoAP request\n");
        return -1;
    }

    // Implement a loop or main event system to keep the program running if necessary.

    return 0;
}