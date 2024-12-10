#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/gcoap.h>
#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <esp_wifi.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <time_units.h>
#include <arpa/inet.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static sock_udp_ep_t server = {
    .family = AF_INET,
    .netif = SOCK_ADDR_ANY_NETIF,
    .port = SERVER_PORT
};

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    ipv4_addr_from_str((ipv4_addr_t *)&server.addr, SERVER_IP);

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        return 1;
    }

    const char *msg = "Hello, COAP Cloud";
    memcpy(pdu.payload, msg, strlen(msg));
    gcoap_finish(&pdu, len, strlen(msg), COAP_FORMAT_TEXT);

    if (gcoap_req_send(buf, pdu.payload_len, &server, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        return 1;
    }

    return 0;
}