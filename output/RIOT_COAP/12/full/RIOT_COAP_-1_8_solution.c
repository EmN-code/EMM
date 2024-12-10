#include "ztimer.h"
#include "xtimer.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "time_units.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "kernel_defines.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "esp_wifi.h"
#include "esp_system.h"

static ssize_t _resp_handler(coap_pkt_t* pdu, void* context)
{
    (void)context; // Suppress the warning of unused variable
    printf("Response received: %.*s\n", (int)pdu->payload_len, (char*)pdu->payload);
    return 0;
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int rc;

    // Initialize remote with CoAP server parameters
    remote.family = AF_INET;
    remote.port = 5683;
    while (1) {
        rc = inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
        if (rc <= 0) {
            printf("inet_pton failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("IP address conversion successful\n");
            break;
        }
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    const char* payload = "Hello, COAP Cloud";
    coap_hdr_set_type(&pdu, COAP_TYPE_CON);
    coap_pkt_append_payload_marker(&pdu);
    coap_pkt_append_payload(&pdu, (uint8_t*)payload, strlen(payload));

    // Send CoAP request
    while (1) {
        ssize_t result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result < 0) {
            printf("gcoap_req_send failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}
