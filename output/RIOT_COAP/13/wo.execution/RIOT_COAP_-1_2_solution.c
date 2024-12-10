#include "ztimer.h"
#include "xtimer.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
 
#define EMQX_URI_PATH "/coap/test"
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_METHOD_POST 2
#define CONFIG_GCOAP_PDU_BUF_SIZE 128
 
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote;
    (void)memo;
    printf("Response received: %s\n", (char *)pdu->payload);
}
 
int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
 
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR);
 
    size_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char *data = "Hello, COAP Cloud";
    memcpy(pdu.payload, data, strlen(data));
    len += strlen(data);
 
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("gcoap_req_send() failed");
    }
 
    return 0;
}