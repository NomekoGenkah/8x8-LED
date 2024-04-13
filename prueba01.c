//https://www.woolseyworkshop.com/2018/06/20/blink-making-an-led-blink-on-a-raspberry-pi/
#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

#define n 8

const int positivos[n] = {26, 19, 13, 6, 5, 11, 9, 10};
const int negativos[n] = {21, 20, 16, 12, 7, 8, 25, 24};
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal) {
   signal_received = signal;
}

int main() {
   if (gpioInitialise() == PI_INIT_FAILED) {
      printf("ERROR: Failed to initialize the GPIO interface.\n");
      return 1;
   }
   for(int i=0; i<n; i++){
      gpioSetMode(positivos[i], PI_OUTPUT);
      gpioSetMode(negativos[i], PI_OUTPUT);
   }

   //gpioSetMode(RedLED, PI_OUTPUT);
   signal(SIGINT, sigint_handler);
   printf("Press CTRL-C to exit.\n");
   while (!signal_received) {
      for(int i=0; i<n; i++){
         for(int j=0; j<n; j++){
            gpioWrite(positivos[i], PI_HIGH);
            gpioWrite(negativos[j], PI_LOW);
            time_sleep(1);
            gpioWrite(positivos[i], PI_LOW);
            gpioWrite(negativos[j], PI_HIGH);
            time_sleep(1);
         }
      }
      //gpioWrite(RedLED, PI_HIGH);
      //time_sleep(1);
      //gpioWrite(RedLED, PI_LOW);
      //time_sleep(1);
   }

   for(int i=0; i<n; i++){
      gpioSetMode(positivos[i], PI_INPUT);
      gpioSetMode(negativos[i], PI_INPUT);
   }
   //gpioSetMode(RedLED, PI_INPUT);
   gpioTerminate();
   printf("\n");
   return 0;
}
