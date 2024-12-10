#include "ztimer.h"  // Provide implementation of timer interface using ztimer
#include "xtimer.h"  // RIOT OS timer utilities
#include "timex.h"   // For time and timer calculations
#include "sys/socket.h" // Provide basic socket definitions
#include "net/sock/util.h" // Provide utilities for sock networking
#include "net/sock/udp.h" // Provide UDP socket interface
#include "net/gcoap.h" // Provide CoAP stack definitions
#include "evtimer_msg.h" // For event timer msg
#include "evtimer_mbox.h" // For event timer mailbox

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(unsigned req_state, coap_pkt_t *pdu, sock_udp_ep_t *remote) {
    (void)remote; // Unused parameter
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("Timeout
");
        return;
    }
    printf("Response received\n");
}

int main(void) {
    // Define and initialize necessary variables
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    // Set remote server details
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    // Prepare CoAP PDU for sending
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, sizeof(MESSAGE));
    gcoap_finish(&pdu, sizeof(MESSAGE), COAP_FORMAT_TEXT);

    // Send CoAP request
    int res = gcoap_req_send(buf, gcoap_get_coap_size(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res < 0) {
        printf("gcoap_req_send() failed with error %d\n", res);
    }

    // Keep the loop running
    while(1){
        ztimer_sleep(ZTIMER_SEC, 10);
    }

    return 0;
}