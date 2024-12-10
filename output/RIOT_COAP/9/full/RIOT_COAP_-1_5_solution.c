// ESP32 CoAP request example using RIOT

// Include necessary headers in descending alphabetical order
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <sys/socket.h>
#include <stddef.h>
#include <netinet/in.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <ztimer.h>
#include <xtimer.h>
#include <net/emcute.h>

// CoAP server details
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"
#define MESSAGE_CONTENT "Hello, COAP Cloud"

// gCoAP handler function
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle response (implementation not provided for brevity)
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    // Convert IP address from string to binary form
    while (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("IP conversion failed, retrying...\n");
        _delay(1);
    }

    // Set remote CoAP server details
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Append payload
    memcpy(coap_payload(&pdu), MESSAGE_CONTENT, sizeof(MESSAGE_CONTENT));

    // Send CoAP request
    while (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Request failed, retrying...\n");
        _delay(1);
    }

    printf("CoAP request sent successfully.\n");
    return 0;
}

void _delay(int seconds) {
   for(int i=0; i<seconds; i++) {
       // Busy-wait
   }
}