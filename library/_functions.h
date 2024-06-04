#ifndef _functions 
#define _functions
//#include<stdlib.h> 		// Para los exit
//#include<sys/types.h> 	// Para la definicion de pid_t
//#include<stdio.h> 		// Para los printf
//#include<unistd.h>		// Para el fork
//#include<errno.h>		// Para el perror
//#include<sys/wait.h>	// Para la definicion de wait
//#include<string.h> 		//Para manejo de cadenas
//#include <sys/stat.h>
//#include <limits.h>
//#include <fcntl.h>
//#include <regex.h>
#include <signal.h>
#include"_semaphore.h" // Funciones de Usuario
#include"_validations.h" // Funciones de Usuario
#include"_shared_memory.h"
#include"_SDL.h"
#include<sys/socket.h> //socket(),bind(),inet_addr()
#include<netinet/in.h> //inet_addr()
#include<arpa/inet.h> //inet_addr()
#define MARGEN_X 861//MARGEN DEL ALTO
#define MARGEN_Y 437//MARGEN DEL ANCHO
#define BITS 32//BITS POR PIXEL (DEFINE LA CALIDAD DE IMAGEN)

pthread_mutex_t mutexSDL;
sem_t *semFicheroMutex;//CONTROLA LAS PETICIONES
int mult,status_;
char buffer[10];
int caller_socket;
pthread_mutex_t mutex,mutexEvent;
//CABECERA DE FUNCIONES//
void iniciarEscudos();
void iniciarEscudos1(FrameShield*frame,int mode,int variable);
void iniciarEscudos2(FrameShield*frame,int mode,int variable);
//void configurarTecla(SDLKey *tecla,char leer[20]);
void startMonitor();
//Funcion del handler de la señal
void esperar(int a);
void setSignal();
//void setConfigKey(int *port,char*ip,SDLKey *derecha,SDLKey *izquierda,SDLKey *disparo);
//Verificar si cuando hay coalision, los dos frames estan incluidos
int include(Frame*f1,Frame*f2);
int verificarColision(Frame *frameBala, Frame naves[5][10],int cant_fil, int cant_col, Frame *expo);
int verificarColision2(Frame *frameBala, Frame *naveRoja, Frame *explo);
int verificarColision3(Frame *frameBalaEnemiga, Frame *frameNave);
int verificarShield(Frame *bala,Frame *frame);
void moverNaves(int cant_filas,int cant, Frame naves[5][10]);
void iniciarNaves(int cant_col,int cant_filas, Frame naves[5][10], int y);
void Send(char*);

#endif
