#include "xtimer.h"
#include "ztimer.h"
#include "stdbool.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "net/af.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "msg.h"
#include "net/sock/util.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "evtimer_mbox.h"
#include "net/emcute.h"
#include "time_units.h"
#include "esp_wifi.h"
#include "timex.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;  // not used
    if (memo->state == GCOAP_MEMO_STATE_COMPLETED) {
        puts("Response received successfully");
    }
    else {
        puts("Failed to receive response");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t res;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    // Convert IP address from string
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to convert server IP address");
        return -1;
    }

    coap_pkt_t pdu;
    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH) < 0) {
        puts("Error: unable to initialize CoAP request");
        return -1;
    }
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    memcpy(pdu.payload, COAP_MESSAGE, strlen(COAP_MESSAGE));
    pdu.payload_len = strlen(COAP_MESSAGE);

    // Sending CoAP request with retry on failure
    while (true) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res > 0) {
            puts("CoAP request sent successfully");
            break;
        } else {
            puts("Error: unable to send CoAP request, retrying...");
            xtimer_sleep(1);
        }
    }
    
    return 0;
}