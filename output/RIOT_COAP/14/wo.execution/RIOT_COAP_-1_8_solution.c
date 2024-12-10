#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "thread_config.h"
#include "timex.h"
#include "time_units.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "sched.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdio.h"

// Define constants
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT (5683)
#define BUFFER_SIZE CONFIG_GCOAP_PDU_BUF_SIZE
#define URI_PATH "/coap/test"

// Function to handle CoAP response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;

    if (memo->state == GCOAP_MEMO_STATE_RESP) {
        printf("Response received:\n");
        printf("CoAP code: %u\n", coap_get_code(pdu));
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    ssize_t res;

    // Convert IP address
    res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Initialize the CoAP PDU
    gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Set payload
    memcpy(pdu.payload, "Hello, COAP Cloud", strlen("Hello, COAP Cloud"));
    pdu.payload_len = strlen("Hello, COAP Cloud");

    // Send the request
    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request\n");
        return 1;
    }

    // Wait indefinitely for the response handler
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}