#include "msg.h"
#include "mutex.h"
#include "stdlib.h"
#include "evtimer_mbox.h"
#include "net/sock/util.h"
#include "timex.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "xtimer.h"
#include "mbox.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "stdio.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "time_units.h"
#include "dht.h"
#include "net/emcute.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "evtimer_msg.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response (could just print a message for this example)
    printf("Response received.\n");
}

int main(void) {
    // Variables
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    size_t len;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Convert server IP to network byte order
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("IP address conversion failed.\n");
        return 1;
    }

    // Initialize CoAP request
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Failed to send CoAP request.\n");
        return 1;
    }

    printf("CoAP request sent successfully.\n");
    return 0;
}