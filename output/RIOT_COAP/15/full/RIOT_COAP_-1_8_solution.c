#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "xtimer.h"

#define COAP_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_MSG "Hello, COAP Cloud"

static const uint8_t *payload = (uint8_t *)COAP_MSG;
static size_t payload_len = sizeof(COAP_MSG) - 1;


static int send_coap_request(void) {
    /* CoAP PDU buffer */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Initialize endpoint */
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF, .port = COAP_SERVER_PORT };

    /* Convert IP string to network format */
    if (inet_pton(AF_INET6, COAP_SERVER_IP, &remote.addr.ipv6) != 1) {
        puts("Error: unable to parse CoAP server address");
        return -1;
    }

    /* Initialize CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(&pdu, COAP_TYPE_NON);
    coap_hdr_set_code(&pdu, COAP_METHOD_POST);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_payload(&pdu, payload, payload_len);

    /* Send CoAP request */
    ssize_t res;

    while (1) {
        res = gcoap_req_send(buf, pdu.payload - buf + payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res < 0) {
            printf("Error sending CoAP request. Retrying in 1 second.\n");
            xtimer_sleep(1);
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}

int main(void) {
    /* Attempt to send CoAP request */
    int res = send_coap_request();

    if (res < 0) {
        printf("Failed to send CoAP request\n");
        return 1;
    }

    return 0;
}
