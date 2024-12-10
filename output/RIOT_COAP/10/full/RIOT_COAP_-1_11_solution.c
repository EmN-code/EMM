#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define COAP_METHOD_POST 2

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Response handling not implemented for simplicity
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    while (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("IP conversion failed, retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    while (1) {
        int rc = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
        if (rc < 0) {
            printf("gcoap_req_init failed: %d\n", rc);
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
        rc = gcoap_req_send(&buf[0], pdu.payload_len + (pdu.payload - buf), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (rc < 0) {
            printf("gcoap_req_send failed: %d\n", rc);
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("CoAP POST request sent successfully.\n");

        break;  // Exit after a successful send
    }

    return 0;
}