#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 64
#define COAP_METHOD_POST 0x02
#define EMQX_URI_PATH  "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define GCOAP_SOCKET_TYPE_UDP 0 
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(const gcoap_pkt_t *pdu, const sock_udp_ep_t *remote,
                   void *context) {
    // Handle response here
    (void)pdu;
    (void)remote;
    (void)context;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t result;
    struct sockaddr_in remote;

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Convert IP address from text to binary form
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.sin_addr);
    
    if (result == 1) {
        remote.sin_family = AF_INET;
        remote.sin_port = htons(5683);

        // Send CoAP request
        gcoap_req_send(buf, CONFIG_GCOAP_PDU_BUF_SIZE, (const sock_udp_ep_t *)&remote, 
                       _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    } else {
        printf("inet_pton failed\n");
    }

    return 0;
}