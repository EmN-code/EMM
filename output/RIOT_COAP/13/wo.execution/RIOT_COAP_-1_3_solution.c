#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 1234  // Define a buffer size constant without collision

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    /* Response handler code to process server response can be added here */
    printf("Response received\n");
}

int main(void)
{
    /* Initialize buffers and endpoint */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    /* using IPv4 correct field name */
    remote.addr.ipv4.u32 = inet_addr("47.102.103.1");

    /* Initialize CoAP packet */
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    len += coap_payload(&pdu, (uint8_t *)"Hello, COAP Cloud", strlen("Hello, COAP Cloud"));

    /* Send CoAP request */
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}