#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread_config.h"
#include "thread.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "sched.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "coap.h"  // Assuming alternative to handle coap functionalities

// Define the remote CoAP server details
#define COAP_SERVER_IP   "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH    "/coap/test"
#define COAP_PAYLOAD     "Hello, COAP Cloud"

// Define a buffer size for CoAP PDU
#define CONFIG_GCOAP_PDU_BUF_SIZE (256U)

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    if (pdu) {
        printf("Response received: \n");
        if (pdu->payload_len) {
            printf("%.*s\n", pdu->payload_len, (char *)pdu->payload);
        }
    }
    else {
        printf("No response received.\n");
    }
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    // convert string IP address to binary form
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error converting IP address\n");
        return 1;
    }

    // Initialize CoAP packet
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len == 0) {
        printf("Error initializing CoAP PDU\n");
        return 1;
    }

    // Add payload
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    len += strlen(COAP_PAYLOAD);

    // Mark the end of the options
    if (coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD) < 0) {
        printf("Error finishing CoAP options\n");
        return 1;
    }

    // Send the request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    return 0;
}