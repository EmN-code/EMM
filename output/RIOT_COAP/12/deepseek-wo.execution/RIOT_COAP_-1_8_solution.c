
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "freertos/FreeRTOS.h"
#include "time_units.h"
#include "esp_system.h"
#include "xtimer.h"
#include "thread.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "mutex.h"
#include "dht_params.h"
#include "stdio.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "dht.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "freertos/task.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "assert.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stddef.h"
#include "mbox.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("CoAP response received\n");
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    int result;

    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_pkt_set_payload(&pdu, (uint8_t *)PAYLOAD, sizeof(PAYLOAD) - 1);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    gcoap_req_send(buf, pdu.offset, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}
