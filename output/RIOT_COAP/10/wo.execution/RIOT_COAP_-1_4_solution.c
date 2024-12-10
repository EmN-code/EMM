#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdint.h"
#include "string.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "esp_system.h"
#include "time_units.h"
#include "timex.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "kernel_defines.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_PORT 5683
#define PAYLOAD "Hello, COAP Cloud"

ssize_t _resp_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)remote; /* not used */
    if (req_state == GCOAP_MEMO_FIN) {
        puts("Response received");
    }
    else {
        puts("No response received");
    }
    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_PORT };
    size_t len;
    
    /* Initialize a CoAP PDU for a POST request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len = coap_get_code(&pdu) + strlen(PAYLOAD);
    
    /* Convert the server address and send the CoAP request */
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    
    /* Let RIOT handle the event loop */
    xtimer_sleep(5);

    return 0;
}