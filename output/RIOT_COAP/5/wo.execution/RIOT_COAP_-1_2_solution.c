#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"

#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "esp_wifi.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define COAP_METHOD_POST 2
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;

    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error: unable to format IP address\n");
        return -1;
    }

    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    snprintf((char *)pdu.payload, pdu.payload_len, "Hello, COAP Cloud");
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending message\n");
        return -1;
    }

    printf("Message sent\n");
    
    return 0;
}
