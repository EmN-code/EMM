#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "time_units.h"
#include "timex.h"
#include "stdint.h"
#include "string.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mutex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static void _response_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, 
                              const sock_udp_ep_t *remote)
{
    (void)remote;
    (void)memo;
    uint8_t code = coap_get_code(pdu);

    if (code == COAP_CODE_204) {
        printf("Success: Received response code 204\n");
    }
    else {
        printf("Error: Received response code %d\n", code);
    }
}

int main(void)
{
    uint8_t buf[128];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes_sent;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Error: Invalid server IP address\n");
        return -1;
    }
    
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (!(memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD)))) {
        printf("Error: Payload copy failed\n");
        return -1;
    }

    bytes_sent = gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error: Failed to send CoAP request\n");
        return -1;
    }

    printf("CoAP POST request sent successfully\n");
    
    return 0;
}