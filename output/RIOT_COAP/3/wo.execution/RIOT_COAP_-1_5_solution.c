#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "ztimer.h"
#include "esp_wifi.h"
#include "dht.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

#define CONFIG_GCOAP_PDU_BUF_SIZE (128U)
#define COAP_PORT (5683)

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Implementation of response handling
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    /* the server's socket address */
    sock_udp_ep_t remote = { .family = AF_INET6, .port = COAP_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &remote.addr);

    coap_pkt_t pdu;
    gcoap_pkt_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, (unsigned)uri_path);

    uint8_t *payload;   
    payload = gcoap_pkt_set_payload(&pdu, &buf[1+uri_path_len+4], (pdu.payload_len - (1+uri_path_len+4)));
    memcpy(payload, "Hello, COAP Cloud", 17);
    ssize_t bytes_sent = gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes_sent <= 0) {
        puts("Error sending CoAP request");
    }
    return 0;
}