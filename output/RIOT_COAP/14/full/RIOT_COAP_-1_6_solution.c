#include <unistd.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <timex.h>
#include <time_units.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <esp_system.h>
#include <esp_wifi.h>

define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    while (1) {
        int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
        if (result != 1) {
            printf("Failed to convert IP address\n");
            xtimer_sleep(1);
            continue;
        }

        int init_res = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
        if (init_res < 0) {
            printf("Failed to initialize CoAP request\n");
            xtimer_sleep(1);
            continue;
        }

        ssize_t len = gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (len <= 0) {
            printf("Failed to send request\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}