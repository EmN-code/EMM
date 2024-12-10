
#include <stdlib.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <dht.h>
#include <mma8x5x_regs.h>
#include <stdatomic.h>
#include <time_units.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <thread.h>
#include <esp_wifi.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <dht_params.h>
#include <stdbool.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <net/ipv6/addr.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <mma8x5x.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        printf("Received response: %d\n", coap_get_code(pdu));
    }
}

int main(void) {
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid server IP address");
        return 1;
    }

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    if (gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH) < 0) {
        puts("Failed to initialize CoAP request");
        return 1;
    }

    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) < 0) {
        puts("Failed to add format option");
        return 1;
    }

    size_t payload_len = strlen(PAYLOAD);
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        puts("Failed to finish CoAP options");
        return 1;
    }

    memcpy(pdu.payload, PAYLOAD, payload_len);

    if (gcoap_req_send(pdu.hdr, pdu.payload - pdu.hdr, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}
