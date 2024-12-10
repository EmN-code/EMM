#include "esp_system.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "mutex.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"

#define COAP_METHOD_POST (0x02)
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"
#define SLEEP_DURATION 1

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t res;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr) != 1) {
        printf("Error: unable to parse server IP\n");
        return 1;
    }

    while (1) {
        ssize_t init_result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        if (init_result < 0) {
            printf("gcoap_req_init failed\n");
            xtimer_sleep(SLEEP_DURATION);
            continue;
        }

        // Set payload
        size_t len = (size_t)snprintf((char *)pdu.payload, CONFIG_GCOAP_PDU_BUF_SIZE - pdu.hdr_len, MESSAGE);
        coap_hdr_set_code(pdu.hdr, COAP_CODE_REQ_POST);
        pdu.payload_len = len;

        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("gcoap_req_send failed, retrying\n");
            xtimer_sleep(SLEEP_DURATION);
            continue;
        }

        printf("Request sent successfully\n");
        break;
    }

    return 0;
}