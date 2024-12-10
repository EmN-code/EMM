#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "string.h"
#include "msg.h"
#include "mutex.h"
#include "xtimer.h"
#include "ztimer.h"

#define COAP_SERVER_IP  "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define PAYLOAD_LEN (sizeof(PAYLOAD) - 1)

static void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    int payload_len;
    const uint8_t *payload = coap_get_payload(pdu, &payload_len);
    printf("Response received: %.*s\n", payload_len, (char *)payload);
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }

    int len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    memcpy(pdu.payload, PAYLOAD, PAYLOAD_LEN);
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res <= 0) {
        printf("Error: failed to send request\n");
        return -1;
    }

    printf("CoAP request sent\n");

    return 0;
}