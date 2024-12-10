#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <unistd.h>

#define COAP_PORT 5683
#define REQUEST_PAYLOAD "Hello, COAP Cloud"
#define COAP_METHOD_POST 2  // POST method in CoAP
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, void *context) {
    if (req_state == GCOAP_MEMO_COMPLETE) {
        printf("Response received\n");
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;
    int result;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    // Convert string IP address to byte representation
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error converting IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Set payload
    coap_payload(&pdu, (uint8_t *)REQUEST_PAYLOAD, sizeof(REQUEST_PAYLOAD) - 1);

    // Send CoAP request
    bytes_sent = gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("Request sent\n");

    return 0;
}
