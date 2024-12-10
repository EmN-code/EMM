#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "unistd.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "timex.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"

#define BUF_SIZE CONFIG_GCOAP_PDU_BUF_SIZE
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received\n");
    } else {
        printf("No response or error\n");
    }
    return 0;
}

int main(void) {
    uint8_t buf[BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert IP address\n");
        return -1;
    }

    if (gcoap_req_init(&pdu, buf, BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH) == NULL) {
        printf("Error: unable to initialize CoAP request\n");
        return -1;
    }

    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_add_payload_marker(&pdu);
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    if (gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send CoAP request\n");
        return -1;
    }
    return 0;
}