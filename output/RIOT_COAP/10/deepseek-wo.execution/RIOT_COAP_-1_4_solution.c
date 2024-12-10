
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_system.h"
#include "mutex.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("gcoap: timeout");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("gcoap: error");
    }
    else if (memo->state == GCOAP_MEMO_RESP) {
        printf("gcoap: response %u.%02u\n", coap_get_code_class(pdu),
               coap_get_code_detail(pdu));
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    int result;

    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        puts("Error converting IP address");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    len += 17;

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
