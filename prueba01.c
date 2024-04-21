#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <time.h>

#define N 8
#define CANTIDAD 3

const int positivos[N] = {26, 19, 13, 6, 5, 11, 9, 10};
const int negativos[N] = {21, 20, 16, 12, 7, 8, 25, 24};
volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal){
   signal_received = signal;
}

void inicializar_gpio(){
    if (gpioInitialise() == PI_INIT_FAILED){
        printf("ERROR: Failed to initialize the GPIO interface.\n");
        return 0;
    }
    for(int i = 0; i < N; i++){
        gpioSetMode(positivos[i], PI_OUTPUT);
        gpioSetMode(negativos[i], PI_OUTPUT);
    }
}

void limpiar_gpio() {
    for(int i = 0; i < N; i++){
        gpioSetMode(positivos[i], PI_INPUT);
        gpioSetMode(negativos[i], PI_INPUT);
    }
    gpioTerminate();
}

void cargar_matriz(int matriz[N][N]){
    FILE *archivo = fopen("matriz.txt", "r");
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if(fscanf(archivo, "%i", &matriz[i][j]) != 1){
                printf("Error al abrir el archivo\n");
                fclose(archivo);
            }
        }
    }
    fclose(archivo);
}

void cargar_animacion(int animacion[CANTIDAD][N][N]){
    char str[20];
    char *nombre = "matriz";

    for(int k=0; k<CANTIDAD; k++){

        snprintf(str, sizeof(str), "%s%d.txt", nombre, k);
        printf("cadena es: %s\n", str);

        FILE *archivo = fopen(str, "r");

        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                if(fscanf(archivo, "%i", &animacion[k][i][j]) != 1){
                    printf("Error\n");
                    fclose(archivo);
                }
            }
        }
		fclose(archivo);
    }
}


void parpadear_leds(int matriz[N][N], int duracion){
    time_t tiempo_inicio, tiempo_actual;
    time(&tiempo_inicio);

    while (difftime(time(&tiempo_actual), tiempo_inicio) < duracion && !signal_received){
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(matriz[i][j] == 1){
                    gpioWrite(positivos[i], PI_HIGH);
                    gpioWrite(negativos[j], PI_LOW);
                    time_sleep(0.001);
                    //gpioDelay(500000);
                    gpioWrite(positivos[i], PI_LOW);
                    gpioWrite(negativos[j], PI_HIGH);
                    time_sleep(0.001);
                    //gpioDelay(500000);
                }
            }
        }
    }
}

void animar(int animar[CANTIDAD][N][N], int duracion){
    time_t tiempo_inicio, tiempo_actual;
    time(&tiempo_inicio);

    while (difftime(time(&tiempo_actual), tiempo_inicio) < duracion && !signal_received){
        for(int k = 0; k < CANTIDAD; k++){
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    if(matriz[CANTIDAD][i][j] == 1){
                        gpioWrite(positivos[i], PI_HIGH);
                        gpioWrite(negativos[j], PI_LOW);
                        time_sleep(0.001);
                        //gpioDelay(500000);
                        gpioWrite(positivos[i], PI_LOW);
                        gpioWrite(negativos[j], PI_HIGH);
                        time_sleep(0.001);
                        //gpioDelay(500000);
                    }
                }
            }
        }
    }
}



int main(){
    int matriz[N][N], duracion=0;
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