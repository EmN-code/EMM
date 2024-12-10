#include <time_units.h>
#include <thread_config.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/nanocoap/cache.h>
#include <net/gcoap.h>
#include <mutex.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <kernel_defines.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <event/thread.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <net/emcute.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

const char *payload = "Hello, COAP Cloud";

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t len;
    sock_udp_ep_t remote = {.family = AF_INET, .port = SERVER_PORT};
    coap_pkt_t pdu;

    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    memcpy(pdu.payload, payload, strlen(payload));
    len += strlen(payload);

    gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}