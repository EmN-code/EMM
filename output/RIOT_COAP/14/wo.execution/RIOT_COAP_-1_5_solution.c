#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sched.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/nanocoap/cache.h>
#include <net/emcute.h>
#include <thread.h>
#include <thread_config.h>
#include <paho_mqtt.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <MQTTClient.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <arpa/inet.h>

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    (void)memo;
    if(pdu == NULL) {
        puts("No response received");
        return;
    }
    puts("Response received");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4);
    remote.port = 5683;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    memcpy(pdu.payload, "Hello, COAP Cloud", sizeof("Hello, COAP Cloud") - 1);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    ssize_t result = gcoap_req_send(buf, pdu.hdr->len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (result > 0) {
        puts("Request sent!");
    } else {
        puts("Failed to send request");
    }

    return 0;
}