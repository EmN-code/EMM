#include <xtimer.h>
#include <timex.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <arpa/inet.h>
#include <dht_params.h>
#include <time_units.h>
#include <esp_wifi.h>

#define COAP_METHOD_POST    0x02
#define COAP_OPT_FINISH_NONE 0x00
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define GCOAP_SOCKET_TYPE_UDP 0

static const char *EMQX_SERVER_IP = "47.102.103.1";

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int create_endpoint(sock_udp_ep_t *remote) {
    remote->family = AF_INET;
    remote->port = 5683;
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote->addr.ipv4);
    return result;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    if (create_endpoint(&remote) <= 0) {
        puts("Failed to convert IP address");
        return -1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    if (len >= 0) {
        len += strlen(PAYLOAD);
        coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    }

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
    }

    return 0;
}