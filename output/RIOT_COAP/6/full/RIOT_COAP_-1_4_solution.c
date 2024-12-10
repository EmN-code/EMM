#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "msg.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"

#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote_endpoint;

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout
");
        return;
    }
    if (pdu->code == COAP_CODE_EMPTY) {
        printf("gcoap: no response received
");
        return;
    }

    printf("gcoap: response received
");
}

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t bytes_sent;
    int res;

    remote_endpoint.family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &remote_endpoint.addr.ipv4);
    remote_endpoint.port = SERVER_PORT;

    while (1) {
        int rc = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
        if (rc == 0) {
            size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

            memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));
            coap_hdr_set_total_payload_len(&pdu, sizeof(PAYLOAD));

            bytes_sent = gcoap_req_send(buf, len, &remote_endpoint, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
            if (bytes_sent > 0) {
                printf("Successfully sent %d bytes
", bytes_sent);
                break;
            } else {
                printf("Failed to send
");
                xtimer_sleep(1);
                continue;
            }
        } else {
            printf("Failed to initialize request
");
            xtimer_sleep(1);
            continue;
        }
    }

    return 0;
}
