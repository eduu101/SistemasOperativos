/*
En esta práctica usando los conceptos mencionados en el marco teórico debemos elaborar un sistema
que permita la compra de boletos de avión. El proceso servidor debe esperar a que un proceso 
cliente se conecte. Una vez que se conecte el proceso cliente, el proceso servidor debe crear un 
proceso hijo para atender al cliente y después debe esperar a otro cliente. Para comunicar los 
procesos se deben utilizar herramientas de comunicación como tuberías o memoria compartida.
Una vez que el cliente esté siendo atendido por el hilo servidor, se le debe pedir la confirmación 
de compra. Mientras que el cliente decide si compra o no los boletos, ningún otro proceso cliente 
puede elegir los asientos que han sido apartados.
Para hacer pruebas de funcionalidad, el proceso servidor debe ejecutarse en una terminal y cada 
uno de los procesos clientes en terminales distintas.
El programa servidor con procesos hijos se le llamará: Programa2-ServidorHijos.c
Todo lo anterior se debe volver a hacer usando hilos en lugar de procesos hijos.
Al programa servidor con hilos se le llamará: Programa2-ServidorHilos.c
Los programas clientes se pueden llamar: Programa2-Cliente.c 
*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>


#define MAX_BUF 1024
#define filas 20
#define cols 6
int numero,numRev;

void reservarAsiento(int,int[filas][cols]);
void llenarMatriz(int[filas][cols]);
void cargarMatriz(char*,int[filas][cols]);
void guardarMatriz(int[filas][cols]);

int main()
{
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf[MAX_BUF];
    int status, pid;
    int bandera=0;
    char resPrimera;
    int sta;
    int Z[filas][cols];
    /* se crea FIFO */
    mkfifo(myfifo, 0666);    

    printf("Es la primera vez que se usa este servidor? (y/n): ");
    scanf("%c", &resPrimera);
    do{
        if(resPrimera=='y' | resPrimera=='Y'){
            llenarMatriz(Z);
            guardarMatriz(Z);
            sta=1;
        }
        else if (resPrimera=='n' | resPrimera=='N')
        {
            printf("Cargando archivo\n");
            cargarMatriz("asientos.txt",Z);
            sleep(2);
            printf("Archivo cargado correctamente\n");
            sleep(2);
            system("clear");
            sta=1;
        }
        else{
            sta=0;
            printf("Respuesta Invalida\n");
        }
    
    }while(sta==0);


    /*Abre y lee la fifo*/  
    while(1){
        sleep(3);
        fd = open(myfifo, O_RDONLY);
        read(fd, buf, MAX_BUF);
        int recibido=(int)strtol(buf,NULL,10);
        if (bandera==recibido)
        {
            printf("Esperando...");
        }
        else
        {
            pid = fork();
            if(pid == 0)                /* hijo */
            {   
                printf("Asiento %s reservado\n", buf);
                reservarAsiento(recibido,Z);
                guardarMatriz(Z);
                bandera=recibido;
            close(fd);
            }
            else                          /* padre */
            { 
            
                        
            }    
        /*wait para el hijo */
        wait(&status); 
        
        }
        
        
    }


    

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
                printf("\nNo se pudo confirmar");
            }
        }
    }
    
}

void llenarMatriz(int matriz[filas][cols]){
    int cont=1;
    for(int i=0;i<filas;i++){
        for(int j=0;j<cols;j++){
            matriz[i][j]=cont;
            cont++;
        }
    }
}
void guardarMatriz(int matriz[filas][cols]){
    FILE *asientos=NULL;
    asientos=fopen("asientos.txt","w");
    if(asientos == NULL ) {
        printf("No fue posible abrir el archivo\n");
    }
    char linea[80];
	for (int i = 0; i < filas; i++) {
        linea[0] = '\0';
		for (int j = 0; j < cols; j++) {
			char buffer[10];
			sprintf(buffer, "%d\t", matriz[i][j]);
			strcat(linea, buffer);
		}
		int len = strlen(linea);
		linea[len - 1] = '\n';
		fputs(linea, asientos);
	}
	fclose(asientos);
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
