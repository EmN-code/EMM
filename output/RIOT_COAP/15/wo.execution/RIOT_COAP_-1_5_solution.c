// Necessary includes 
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128  // Correct size definition
#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define EMQX_SERVER_IP "47.102.103.1"

static sock_udp_ep_t remote;
static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
coap_pkt_t pdu;

void _resp_handler(unsigned req_state, coap_pkt_t *pdu, void *context) {
    (void)context;
    if (req_state == GCOAP_MEMO_RESP) {
        printf("Success: Received response\n");
    }
    else {
        printf("Error: Did not receive response\n");
    }
}

int main(void) {
    // Convert IP address string to byte-represented IP address
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: Unable to parse IP address\n");
        return -1;
    }
    remote.port = 5683;

    // Initialize CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    // Check for CoAP request initialization error
    if (len <= 0) {
        printf("Error: Unable to initialize CoAP request\n");
        return -1;
    }

    // Copy payload into PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Send CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: Unable to send CoAP request\n");
        return -1;
    }

    printf("CoAP request sent successfully\n");

    return 0;
}