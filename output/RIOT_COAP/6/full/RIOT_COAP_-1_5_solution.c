#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdatomic.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <msg.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <timex.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    printf("Starting CoAP POST request to %s at URI %s\n", EMQX_SERVER_IP, COAP_URI_PATH);

    /* Initialize CoAP request */
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t len;

    while (1) {
        int rc = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
        if (rc != 1) {
            printf("Failed to convert IP address. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("IP address conversion succeeded.\n");
            break;
        }
    }

    remote.port = 5683;

    len = gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_total_payload_len(&pdu, strlen(PAYLOAD));
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    while (1) {
        ssize_t bytes = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (bytes <= 0) {
            printf("Failed to send CoAP request. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("CoAP request sent successfully, %zd bytes\n", bytes);
            break;
        }
    }

    return 0;
}
