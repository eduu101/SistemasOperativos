#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define filas 20
#define cols 6
int numero,numRev;

void imprimirMenu();
void reservarAsiento(int,int[filas][cols]);
void imprimirMatriz(int[filas][cols]);
void cargarMatriz(char*,int[filas][cols]);


int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";

    char buf[6],aux[4],aux2[4];

    char resPrimera,resVolver;
    int sta,resMenu,asi;
    int Z[filas][cols];
   
    printf("Cargando archivo...\n");
    cargarMatriz("asientos.txt",Z);
    printf("Archivo cargado correctamente\n");
    sleep(4);
    system("clear");

    do{
        system("clear");
        cargarMatriz("asientos.txt",Z);
        imprimirMenu();
        scanf("%i",&resMenu);
        switch(resMenu){
            case 1:{
                imprimirMatriz(Z);
                break;
            }
            case 2:{
                int bandera=0,val=0;
                printf("¿Que asiento deseas verificar?:");
                scanf("%i",&asi);
                for(int i=0;i<filas;i++){
                    for(int j=0;j<cols;j++){
                        bandera++;
                        if(Z[i][j]==asi){
                            printf("\nAsiento Disponible\n");
                            val=1;
                        }
                        else if(bandera==120 && val==0){
                            printf("\nNo disponible\n");
                        }
                    }
                }
                break;
            }
            case 3:{
                int bandera=0,val=0,conf=0;
                char resaux;
                printf("¿Que asiento deseas reservar?:");
                scanf("%i",&asi);
                for(int i=0;i<filas;i++){
                    for(int j=0;j<cols;j++){
                        bandera++;
                        if(Z[i][j]==asi){
                            printf("\nAsiento Disponible\n");
                            val=1;
                            if (val==1)
                            {
                                printf("Confirmar asiento?(y/n):");
                                scanf(" %c",&resaux);
                                if (resaux=='y'||resaux=='Y')
                                {
                                    conf=1;
                                }
                                else
                                {
                                    conf=0;
                                    break;
                                }
                            }

                        }
                        else if(bandera==120 && val==0){
                            printf("\nNo disponible\n");
                            break;
                        }
                    }
                }
                if (val==1 && conf==1)
                {
                /* escribir en FIFO */
                reservarAsiento(asi,Z);
                sprintf(aux,"%i",asi);
                fd = open(myfifo, O_WRONLY);
                write(fd,aux, sizeof(aux));
                close(fd);
                break;
                }
            }
            case 4:{
                exit(0);
                break;
            }
            default:{
                printf("error: opción invalida.");
                break;
            }
        }
        fflush(stdin);
        printf("Deseas volver al menu?(y/n): ");
        scanf("%c",&resVolver);

    }while (resVolver=='y' || resVolver=='Y');
    
        /* remove the FIFO */
    unlink(myfifo);

	return 0;
}
void reservarAsiento(int asiento,int matriz[filas][cols]){
    for(int i=0;i<filas;i++){
        for(int j=0;j<cols;j++){
            numRev=matriz[i][j];
            if(numRev==asiento){
                matriz[i][j]=0;
            }
            else if(((6*i)+j+1) == asiento && matriz[i][j]==0){
                printf("\nNo se pudo confirmar\n");
            }
        }
    }
    
}
void imprimirMenu(){
    printf("\t\t*** Menu ***\t\t\n");
    printf("Opcion 1: Ver asientos\n");
    printf("Opcion 2: Verificar asiento\n");
    printf("Opcion 3: Reservar asiento\n");
    printf("Opcion 4: Salir\n\n");
    printf("Opcion: ");
}
void imprimirMatriz(int matriz[filas][cols]){
    for(int i=0;i<filas;i++){
        for(int j=0;j<cols;j++){
            printf("%d\t",matriz[i][j]);
        }
        printf("\n");
    }
}
void cargarMatriz(char *fileName, int matriz[filas][cols]){
    FILE *fp = fopen(fileName, "rb");
	if (fp == NULL) {
		printf("Error al cargar el archivo.");
	}
	while(numero<120){
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < cols; j++) {
                fscanf(fp,"%d",&numero);
                matriz[i][j]=numero;
		    }
	    }
    }
	fclose(fp);
}