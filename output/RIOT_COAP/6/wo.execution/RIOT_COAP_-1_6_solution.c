#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "sched.h"
#include "sock/udp.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/nanocoap/cache.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "sys/socket.h"
#include "stdio.h"
#include "stddef.h"
#include "paho_mqtt.h"
#include "nanocoap/cache.h"
#include "kernel_defines.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Handle the response
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response handler called\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to convert server IP address\n");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    printf("Request initialized\n");
    if (len == 0) {
        printf("Error: unable to initialize request\n");
        return -1;
    }

    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    memcpy(coap_pkt_get_payload(&pdu), COAP_PAYLOAD, sizeof(COAP_PAYLOAD));
    printf("Payload set\n");

    ssize_t res = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error: unable to send request\n");
        return -1;
    }
    printf("Request sent\n");

    return 0;
}