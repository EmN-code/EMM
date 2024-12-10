#include udp-socket.h
#include tcp-socket.h
#include sys/timer.h
#include sys/rtimer.h
#include sys/process.h
#include sys/pt.h
#include sys/etimer.h
#include sys/clock.h
#include sys/ctimer.h
#include net/routing/routing.h
#include net/ipv6/uiplib.h
#include net/ipv6/uip.h
#include net/ipv6/uip-sr.h
#include net/ipv6/uip-nameserver.h
#include net/ipv6/uip-icmp6.h
#include net/ipv6/uip-ds6-route.h
#include net/ipv6/uip-ds6-nbr.h
#include net/ipv6/uip-ds6.h
#include net/ipv6/uip-udp-packet.h
#include net/ipv6/tcpip.h
#include net/ipv6/tcp-socket.h
#include net/ipv6/simple-udp.h
#include net/ipv6/psock.h
#include net/ipv6/uip-ds6.h
#include net/ipv6/uip-nameserver.h
#include net/ipv6/udp-socket.h
#include net/netstack.h
#include coap-request-state.h
#include coap-observe.h
#include coap-observe-client.h
#include contiki.h
#include contiki-net.h
#include contiki-lib.h
#include contiki-default-conf.h
#include contiki-conf.h
dev/etc/rgb-led/rgb-led.h

PROCESS(sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&sensor_process);

static struct etimer timer;

PROCESS_THREAD(sensor_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize RGB LED
    rgb_led_init();

    // Periodically read sensor and turn on RGB LED
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Assume we simulate reading from a DHT11 sensor
        printf("Reading from DHT11 sensor...\n");

        // Simulate controlling the RGB LED
        rgb_led_set(RGB_LED_RED);
        printf("RGB LED turned on.\n");
        
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&sensor_process, NULL);
    while(1) {
        process_run();
    }
    return 0;
}