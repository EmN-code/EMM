#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define CONFIG_GCOAP_PDU_BUF_SIZE 256

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                             const sock_udp_ep_t *remote) {
    (void)remote;
    printf("Response received\n");
    printf("payload length: %u\n", (unsigned)pdu->payload_len);
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = 5683;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.hdr->code = COAP_METHOD_POST;
    ssize_t bytes_sent = gcoap_req_send(buf, pdu.payload_len + (pdu.payload - buf),
                                        &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent < 0) {
        printf("Error sending packet\n");
    }
    return 0;
}
