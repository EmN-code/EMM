#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "unistd.h"
#include "msg.h"
#include "mutex.h"
#include "string.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "kernel_defines.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "net/ipv6/addr.h"
#include "time_units.h"
#include "stdatomic.h"
#include "dht.h"
#include "dht_params.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128
#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 5683

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    gcoap_request_memo_t req;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    memset(&buf, 0, sizeof(buf));
    memset(&remote, 0, sizeof(remote));

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = EMQX_SERVER_PORT;
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_add_payload_marker(&pdu);

    char *payload = "Hello, COAP Cloud";
    if (coap_payload(&pdu) != NULL) {
        memcpy(coap_payload(&pdu), payload, strlen(payload));
        coap_hdr_set_total(&pdu, strlen(payload));
    }

    while (1) {
        bytes_sent = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes_sent <= 0) {
            printf("Error sending CoAP request... retrying\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}