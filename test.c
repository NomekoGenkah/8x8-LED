#include <stdio.h>

#define N 8
#define CANTIDAD 3

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

void imprimir_animacion(int animacion[CANTIDAD][N][N]){
    for(int k=0; k<CANTIDAD; k++){
    	printf("\n");
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                printf("%2i", animacion[k][i][j]);
            }
            printf("\n");
        } 
        printf("\n");
    }
}





int main() {
    int animacion[CANTIDAD][N][N];
    cargar_animacion(animacion);
    imprimir_animacion(animacion);


    return 0;
}

