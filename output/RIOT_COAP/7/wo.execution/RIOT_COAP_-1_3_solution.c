#include "sys/socket.h"
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/mqttsn.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

#define COAP_PORT 5683
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Received response\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    if (inet_pton(remote.family, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: IP address conversion failed\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);
    if (coap_opt_add_format(&pdu, COAP_FORMAT_TEXT) < 0) {
        puts("Error adding option\n");
        return 1;
    }

    ssize_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    const char *payload = "Hello, COAP Cloud";
    len += coap_payload(&pdu, (uint8_t *)payload, strlen(payload));

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("gcoap_req_send() failed\n");
        return 1;
    }

    return 0;
}