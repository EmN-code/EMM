#include <thread.h>
#include <net/sock/util.h>
#include <dht_params.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <arpa/inet.h>
#include <esp_wifi.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <mma8x5x.h>

#define COAP_URI_PATH "/coap/test"
#define COAP_PORT 5683

static const char *SERVER_IP = "47.102.103.1";
static const char *coap_message = "Hello, COAP Cloud";

gcoap_request_memo_t _resp_handler;

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    puts("Response received");
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;

    bytes = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (bytes < 0) {
        puts("Failed to initialize request");
        return 1;
    }

    memcpy(pdu.payload, coap_message, strlen(coap_message));
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UNDEF);

    return 0;
}