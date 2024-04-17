#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <time.h>

#define n 8

const int positivos[n] = {26, 19, 13, 6, 5, 11, 9, 10};
const int negativos[n] = {21, 20, 16, 12, 7, 8, 25, 24};
volatile sig_atomic_t signal_received = 0;

void cargar_matriz(int matriz[n][n]){
    FILE *archivo = fopen("matriz.txt", "r");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(fscanf(archivo, "%i", &matriz[i][j]) != 1){
                printf("Error al abrir el archivo\n");
                fclose(archivo);
            }
        }
    }
    fclose(archivo);
}

void sigint_handler(int signal){
   signal_received = signal;
}

void inicializar_gpio(){
    if (gpioInitialise() == PI_INIT_FAILED){
        printf("ERROR: Failed to initialize the GPIO interface.\n");
        return 0;
    }
    for(int i = 0; i < n; i++){
        gpioSetMode(positivos[i], PI_OUTPUT);
        gpioSetMode(negativos[i], PI_OUTPUT);
    }
}

void parpadear_leds(int matriz[n][n], int duracion){
    time_t tiempo_inicio, tiempo_actual;
    time(&tiempo_inicio);

    while (difftime(time(&tiempo_actual), tiempo_inicio) < duracion && !signal_received){
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                if(matriz[i][j] == 1){
                    gpioWrite(positivos[i], PI_HIGH);
                    gpioWrite(negativos[j], PI_LOW);
                    gpioDelay(500000);
                    gpioWrite(positivos[i], PI_LOW);
                    gpioWrite(negativos[j], PI_HIGH);
                    gpioDelay(500000);
                }
            }
        }
    }
}

void limpiar_gpio() {
    for(int i = 0; i < n; i++){
        gpioSetMode(positivos[i], PI_INPUT);
        gpioSetMode(negativos[i], PI_INPUT);
    }
    gpioTerminate();
}

int main(){
    int matriz[n][n], duracion=0;
    prinntf("Ingrese segundos:\n");
    scanf("%d", &duracion);

    signal(SIGINT, sigint_handler);
    printf("Press CTRL-C to exit.\n");

    cargar_matriz(matriz);
    inicializar_gpio(matriz); 

    parpadear_leds(matriz, duracion);
    limpiar_gpio();
    
    printf("\n");
    return 0;
}