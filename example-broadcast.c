/**
 * \file
 *         Example application for the motes to run. Adapted from Contiki's 
 *         examples/rime/broadcast.c.
 * \author
 *         Mat Wymore <mlwymore@gmail.com>
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include "powertrace.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"
#include "net/netstack.h"

#include <stdio.h>

/*  How often the node should broadcast a packet, in seconds. Automatically set by shell script.  */
#define DATA_ARRIVAL_INTERVAL 25
/*  How many packets the node should send in the simulation. Automatically set by shell script.  */
#define NUM_PACKETS_TO_SEND 250

static int sentCounter = 0;
static int attemptedCounter = 0;

/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  //printf("message received from %d.%d: '%s'\n",
  //       from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}

static void
broadcast_sent(struct broadcast_conn *bc, int status, int num_tx)
{
  //printf("message sent\n");
  if (status == MAC_TX_OK) {
    sentCounter++;
  }
  /*if (sentCounter % 8 == 0) {
    printf("Reliability stats: %d attempted, %d sent\n", attemptedCounter, sentCounter);
  }*/
  
}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv, broadcast_sent};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
  static struct etimer et;
  static clock_time_t start_time;
  static uint32_t interval;
  //static int i = 0;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  start_time = clock_time();

  /* Start powertracing, once every 1 seconds. */
  //powertrace_start(CLOCK_SECOND * 1);

  printf("data arrival interval: %d, start time: %lu\n", DATA_ARRIVAL_INTERVAL, (unsigned long)start_time);

  broadcast_open(&broadcast, 129, &broadcast_call);

  while(1) {

    /* Delay a few seconds */
    //interval = CLOCK_SECOND * (DATA_ARRIVAL_INTERVAL - 1) + random_rand() % (CLOCK_SECOND * 2);
    interval = CLOCK_SECOND * DATA_ARRIVAL_INTERVAL;
    etimer_set(&et, interval);
    //printf("data arrival timer set for %lu\n", interval); 
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    //if (clock_time() - start_time < CLOCK_SECOND * 3600) {
    if (attemptedCounter < NUM_PACKETS_TO_SEND) {
      packetbuf_copyfrom("Hello", 6);
      //for (i = 0; i < 1; i++) {
        broadcast_send(&broadcast);
        attemptedCounter++;
        //printf("message queued\n");
      //}
    }
    else if (sentCounter == attemptedCounter || clock_time() > CLOCK_SECOND * DATA_ARRIVAL_INTERVAL * NUM_PACKETS_TO_SEND * 2) {
      //printf("done with program\n");
      break;
    }
  }

  printf("RELIABILITY %d %d\n", attemptedCounter, sentCounter);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
