#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"

#define EMQX_SERVER_IP          "47.102.103.1"
#define EMQX_URI_PATH          "/coap/test"
#define EMQX_PORT              5683
#define PAYLOAD                "Hello, COAP Cloud"

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, 
                          const sock_udp_ep_t *remote) {
    (void)remote;
    if (req_state == GCOAP_MEMO_FIN) {
        printf("Received response with code %d\n", coap_get_code_raw(pdu));
    }
}

int main(void) {
    gcoap_req_init_t req_init;
    gcoap_request_param_init(&req_init);
    
    sock_udp_ep_t remote = { .port = EMQX_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD) - 1);

    gcoap_socket_type_t tl_type = GCOAP_SOCKET_TYPE_UDP;
    ssize_t res = gcoap_req_send(buf, sizeof(PAYLOAD) - 1, &remote, _resp_handler, NULL, tl_type);

    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    return 0;
}