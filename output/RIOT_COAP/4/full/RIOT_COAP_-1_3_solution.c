#include "net/sock/util.h"
#include "mutex.h"
#include "string.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "thread.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "net/sock/udp.h"
#include "net/emcute.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static gcoap_resp_handler_t _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)pdu;
    (void)remote;
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout receiving response\n");
    }
    return GCOAP_MEMO_CONTINUE;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    // Convert server IP from text to network format
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse server IP\n");
        return 1;
    }
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (!len) {
        printf("Error: unable to initialize CoAP request\n");
        return 1;
    }

    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    len += strlen(COAP_PAYLOAD);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent successfully\n");
    return 0;
}