#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "thread_config.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/nanocoap/cache.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "unistd.h"
#include "stdatomic.h"
#include "time_units.h"

// Define constants
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define COAP_METHOD_POST 2
#define GCOAP_SOCKET_TYPE_UDP 0

static void _resp_handler(unsigned req_state) {
    if(req_state == GCOAP_MEMO_RESP) {
        printf("Response received!\n");
    }
    else {
        printf("No response received.\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return 1;
    }

    // Initialize CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    const char* payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    // Send request
    if (gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    puts("CoAP request sent");
    return 0;
}