#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netinet/tcp.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

void _resp_handler(unsigned req_state, coap_pkt_t* pdu, const sock_udp_ep_t *remote) {
    (void)req_state;
    (void)pdu;
    (void)remote;
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };

    int res;
    while (1) {
        res = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (res <= 0) {
            if (res == 0) {
                puts("Invalid IP address format");
            } else {
                perror("inet_pton");
            }
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    while (1) {
        int init_res = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
        if (init_res < 0) {
            puts("Failed to initialize CoAP request");
            xtimer_sleep(1);
            continue;
        }

        unsigned payload_size = strlen(PAYLOAD);
        if (coap_hdr_set_payload_len(&pdu, payload_size) < 0) {
            puts("Failed to set CoAP payload length");
            xtimer_sleep(1);
            continue;
        }

        memcpy(pdu.payload, PAYLOAD, payload_size);

        if (gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
            puts("Failed to send CoAP request");
            xtimer_sleep(1);
            continue;
        }

        puts("CoAP request sent successfully");
        break;
    }

    return 0;
}