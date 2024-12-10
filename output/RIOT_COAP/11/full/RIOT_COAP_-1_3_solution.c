#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "net/af.h"
#include "netinet/in.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "arpa/inet.h"

#define COAP_SERVER_ADDR "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"
#define ATTEMPT_DELAY (1U * US_PER_SEC)

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response from the CoAP server
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout communicating with CoAP server\n");
    } else if (memo->state == GCOAP_MEMO_ERR) {
        printf("Error in CoAP communication\n");
    } else {
        printf("Received response from CoAP server\n");
    }
}

static int _send_coap_request(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_ADDR, &remote.addr);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int tries = 0;

    while (1) {
        int rc = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
        if (rc < 0) {
            printf("Failed to initialize CoAP request\n");
            return -1;
        }

        memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
        rc = coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
        if (rc != 0) {
            printf("Failed to set CoAP header\n");
            return -1;
        }

        rc = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc < 0) {
            printf("Failed to send CoAP request\n");
            xtimer_sleep(ATTEMPT_DELAY);
            tries++;
            if (tries >= 5) {
                printf("Exceeded maximum retry attempts\n");
                return -1;
            }
            continue;
        }

        printf("CoAP request sent successfully\n");
        break;
    }

    return 0;
}

int main(void) {
    puts("RIOT CoAP example application");

    while (1) {
        if (_send_coap_request() == 0) {
            break;
        }
        xtimer_sleep(ATTEMPT_DELAY);
    }

    return 0;
}