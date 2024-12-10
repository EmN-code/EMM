#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"

static const char *EMQX_URI_PATH = "/coap/test";
static const char *SERVER_IP = "47.102.103.1";
#define SERVER_PORT (5683)
#define BUF_SIZE (128)

gcoap_request_memo_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handler logic here
    printf("Response received\n");
    return 0;  // Return an appropriate response
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    ssize_t len;

    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_IP) == NULL) {
        printf("Error: Could not parse destination address\n");
        return 1;
    }

    len = gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    if (len < 0) {
        printf("Error: gcoap_req_init failed\n");
        return 1;
    }

    if (coap_set_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", 17) < 0) {
        printf("Error: coap_set_payload failed\n");
        return 1;
    }

    // Retry logic
    while (1) {
        if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) >= 0) {
            printf("Request sent successfully\n");
            break;
        } else {
            printf("Send failed, retrying...\n");
            xtimer_sleep(1);
        }
    }

    return 0;
}
