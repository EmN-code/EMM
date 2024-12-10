#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <dma8x5x.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <timex.h>
#include <mma8x5x_params.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mutex.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

gcoap_req_data_t req_buf;
sock_udp_ep_t remote;

void _resp_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len) {
    printf("Response received\n");
}

int main(void){
    // Initialize CoAP remote endpoint
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Initialize CoAP request
    coap_pkt_t pkt;
    gcoap_req_init(&pkt, req_buf.buf, sizeof(req_buf.buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(req_buf.buf + pkt.hdr_len, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    size_t req_len = pkt.hdr_len + sizeof(COAP_MESSAGE);

    // Send CoAP request
    ssize_t res;
    while (1) {
        res = gcoap_req_send(req_buf.buf, req_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res <= 0) {
            printf("Failed to send request, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Request sent\n");
        break;
    }

    return 0;
}