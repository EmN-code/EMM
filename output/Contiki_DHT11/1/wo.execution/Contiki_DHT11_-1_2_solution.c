#include net/ipv6/uip-ds6.h
#include net/ipv6/simple-udp.h
#include net/ipv6/uip-nameserver.h
#include contiki-conf.h
#include net/ipv6/uip-ds6-nbr.h
#include net/mac/framer/framer.h
#include net/ipv6/uip-udp-packet.h
#include net/routing/routing.h
#include net/ipv6/psock.h
#include net/ipv6/uiplib.h
#include net/ipv6/uip.h
#include contiki-net.h
#include net/ipv6/uip-ds6-route.h
#include contiki.h
#include net/ipv6/udp-socket.h
#include net/ipv6/tcpip.h
#include net/ipv6/tcp-socket.h
#include contiki-default-conf.h
#include net/ipv6/uip-sr.h
#include net/ipv6/uip-icmp6.h
#include contiki-net.h
#include contiki.h
#include contiki-lib.h

#include <stdio.h>
#include <unistd.h>

PROCESS(dht11_process, "DHT11 Reader");
AUTOSTART_PROCESSES(&dht11_process);

// Placeholder function to simulate DHT11 reading
static int read_dht11_sensor(void) {
    // Simulate reading temperature and humidity
    int temperature = 25; // Dummy temperature
    int humidity = 60;    // Dummy humidity
    printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
    return 0;
}

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11_sensor();
        etimer_reset(&timer);
    }

    PROCESS_END();
}