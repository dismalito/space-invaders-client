#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
//#include "../library/_SDL.h"
#include <string.h>
#include"../library/_functions.h"
#define MARGEN_X 861//MARGEN DEL ALTO
#define MARGEN_Y 437//MARGEN DEL ANCHO
#define BITS 32//BITS POR PIXEL (DEFINE LA CALIDAD DE IMAGEN)
#define FPS 30

////////////////////////////////////////////////////////////////////////////////////////////////////////
char *ip;
int port,terminar=0,status2=0,mult2=MARGEN_X-220,bala_activa=0,contExplo=50,finSplash=0;
SDLKey derecha,izquierda,disparo;
pthread_t thread1,thread2,thread3;


pthread_t tidR,tidE;

int terminarReceive=0;
int terminarSend=0;


void *dibujar(int *salir);
void *monitor(int *salir);//Un hijo para monitor el cliente(como un demonio)
void *events(int *salir);
/////////////////////////////////////// MAIN ///////////////////////////////////////


//CREA SOCKET DE CONEXION
void SocketConnect(char*ip,int puerto){
        char buffer[200];
        unsigned short int listen_port=0;
        unsigned long int listen_ip_address=0;
        struct sockaddr_in listen_address;
        //Creación del socket
        caller_socket=socket(AF_INET,SOCK_STREAM,0);
        if(caller_socket==-1){
                printf("No sea ha podido establecer conexión. Programa finalizado");
                exit(EXIT_FAILURE);
        }        
        listen_address.sin_family=AF_INET;
        listen_port=htons(puerto);//Indico puerto
        listen_address.sin_port=listen_port;
        listen_ip_address=inet_addr(ip);
        listen_address.sin_addr.s_addr=listen_ip_address;
        bzero(&(listen_address.sin_zero),8);
        if(connect(caller_socket,(struct sockaddr*)&listen_address,sizeof(struct sockaddr))==-1){//Me conecto al servidor
                printf("No sea ha podido establecer conexión. Programa finalizado");
                exit(EXIT_FAILURE);
        }
        
}

//FUNCION PARA RECIBIR PAQUETES//MANEJADA POR UN THREAD
void *recibiendoInfo(int *exitClient){
        char buffer[200];
        while(strcmp(buffer,"exit")!=0 && terminarReceive==0){
                if(recv(caller_socket,buffer,sizeof(char)*200,0)!=-1){//RECIBO RESULTADO
                        printf("[%s]\n",buffer);
                        if(strcmp(buffer,"exit")==0)
                                terminarReceive=1;
                        if(strcmp(buffer,"start")==0){
                                finSplash=1;
                                pthread_mutex_unlock(&mutexSDL);//PERMITO DIBUJAR
                        }
                }
                else{//SI TUVE ERROR AL RECIBIR
                        pthread_cancel(tidE);//CIERO THREAD DE ENVIOS
                        printf("Se ha perdido la conexión con el servidor\n");
                        terminarReceive=1;
                        close(caller_socket);
                }
        }
        close(caller_socket);
        pthread_cancel(tidE);//CIERO THREAD DE ENVIOS     
	pthread_exit(exitClient);
}

//FUNCION PARA ENVIAR PAQUETES//MANEJADA POR UN THREAD
void *enviandoInfo(int *exitClient){
        while(terminarSend==0){//MIENTRAS EL JUEGO NO TERMINE
                //PIDE MUTEX
                pthread_mutex_lock(&mutex);
                
                if(send(caller_socket,buffer,sizeof(char)*200,0)==-1){//SI SE ENVIO CORRECTAMENTE ESPERO RESULTADO
                        //pthread_cancel(tidR);//CIERRO THREAD DE RECIBIR
                        printf("Se ha perdido la conexión con el servidor\n");
                        terminarSend=1;
                }
                if(strcmp(buffer,"exit")==0)
                        terminarSend=1;
        }
        pthread_exit(exitClient);
}

int main(int argc, char *argv[]){

printf("Ejecutando Validaciones Preliminares...\n");
/***********VALIDACIONES*********/
if (validacionGeneral(argc, argv) == 1)
	exit(EXIT_FAILURE);

printf("IP a donde se conectara: %s \n",Param.IP_DNS);
printf("Puerto: %d \n",Param.puerto);
printf("Tecla Derecha: %d \n",Param.tecla_derecha);
printf("Tecla Disparo: %d \n",Param.tecla_disparo);
printf("Tecla Izquierda: %d\n",Param.tecla_izquierda);

/********FIN VALIDACIONES********/

pthread_mutex_init(&mutex,NULL);//INICIA EL MUTEX
pthread_mutex_init(&mutexEvent,NULL);//INICIA EL MUTEX

       setSignal();
       int salir;
       mult=1;
       status_=0;
       

startMonitor();
//setConfigKey(&port,ip,&derecha,&izquierda,&disparo);

//asigno los parametros del archivo de configuracion a las variables
port = Param.puerto;
ip = Param.IP_DNS;
derecha = Param.tecla_derecha;
izquierda = Param.tecla_izquierda;
disparo = Param.tecla_disparo;
SocketConnect(ip,port);//CONECTO Y SOCKET DISPONIBLE
printf("Ingrese un nick:");
gets(buffer);
Send(buffer);


SDLRun(MARGEN_X,MARGEN_Y,BITS,"SPACE INVADERS");
pthread_mutex_init(&mutexSDL,NULL);//INICIA EL MUTEX
pthread_create(&thread1, NULL, (void *)&dibujar,(void*)&salir);//CREO UN THREAD PARA DIBUJAR
pthread_create(&thread2, NULL, (void *)&events,(void*)&salir);//CREO UN THREAD PARA CONFIGURAR TECLAS
pthread_create(&tidR, NULL, (void *)&recibiendoInfo,(void*)&salir);//CREO UN THREAD PARA RECIBIR MENSAJES
pthread_create(&tidE, NULL, (void *)&enviandoInfo,(void*)&salir);//CREO UN THREAD PARA ENVIAR MENSAJES  

pthread_join(thread1, NULL);
pthread_join(thread2, NULL);
//pthread_mutex_destroy(&mutexSDL);
return 0;
}

void *events(int *salir){
        int status_exit=0,status_stop=0;
        int salir2=0;
       pthread_mutex_lock(&mutexEvent);
        //PRIMER LOOP PARA SPLASH//DETECTO EVENTOS
        while(!finSplash){
                //// INDICO QUE HAY UN NUEVO EVENTO ////
                SDL_WaitEvent(&event);
                if(event.type==SDL_QUIT){//SE PRESIONA LA X DE SALIR//HAY QUE LIBERAR RECURSOS
                        SDL_Quit();
                        exit(1);
                }
                if(event.type==SDL_MOUSEMOTION)//EVENTO DE MOUSE MOVIENDOSE
                        check_mouse_position(event.motion.x,event.motion.y,&status_exit,&status_stop);
                if(status_exit==1 && event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT){
                        //finSplash=1;//SE PULSA BOTON DE START
                        Send("buscar");
                        //SEND DE NADA
                        usleep(500000);
                        Send("nada");//SEND
               
                        //finSplash=1;

                }
                if(status_stop==1 && event.type==SDL_MOUSEBUTTONDOWN && event.button.button==SDL_BUTTON_LEFT){
                        SDL_Quit();//SE PULSA EL BOTON DE EXIT
                        Send("exit");
		        sleep(2);
		        close(caller_socket);
        	        exit(1);
        	}
        }
        /// TERMINA EVENTOS DE SPLASH
        
        //LOOP PARA SALA DE ESPERA Y JUEGO (ALTERNA ENTRE LOS DOS)
        
        
        
        
        //TERCER LOOP PARA EL JUEGO
        
        
        
        
        while(!terminar){
                SDL_PollEvent(&event);
                teclas = SDL_GetKeyState(NULL);
                int incremento = 4;
		if(teclas[SDLK_ESCAPE]){
		        Send("exit");
		        sleep(2);
		        close(caller_socket);
		        terminar=1;
		}
		if(teclas[izquierda] && frameNave.position.x>10){frameNave.position.x-=incremento;}
                if(teclas[derecha] && frameNave.position.x<MARGEN_X-210){frameNave.position.x+=incremento;}                     
                if(event.type==SDL_QUIT){//SE PRESIONA LA X DE SALIR
                        Send("exit");                        
		        sleep(2);
		        close(caller_socket);
		        terminar=1;
		}                        
		//// SI SE PRECIONA SPACE Y NO TENGO BALA ACTIVA LA CREA ////               
		if(teclas[disparo] && bala_activa==0){
				//// SETEA BALA ACTIVA EN 1 PARA QUE LUEGO SE PUEDA DIBUJAR SOBRE EL BUFFER ////
				bala_activa=1;
				//// AL CREAR LA BALA LA POSICIONA EN EL CAÑON DE LA NAVE ////
				setPosition(&frameBala, frameNave.position.x+8, frameNave.position.y-7);
		}  
                        usleep(30000);
                        
        }
        *salir=1;
 pthread_exit(salir);
}

void *dibujar(int *salir){
        // PRIMERO LOOP PARA SPLASH
        while(!finSplash){
                pthread_mutex_lock(&mutexSDL);
                //SDL_BlitSurface(fondoSala,NULL,SDLBuffer,NULL);
                addScreen(&btn_stop,BOTON);//DIBUJO BOTONES
                addScreen(&btn_exit,BOTON);
                showScreen();
        }


        int bala_activaEnemiga=0,nave_activa=0,pts=0,puntaje=0;
        int i,j,p,div=0;	

        //INICIALIZO NAVES MARCIANAS
        iniciarNaves(cant_naves,cant_filas,naves,150);
        //INICIALIZO LOS 8 ESCUDOS
        iniciarEscudos();

        //GAMELOOP
        while(!terminar){

                int startTimer;
                int endTimer;
                int deltaTime;
                
                startTimer = SDL_GetTicks(); 

                int t,navesActivas=0;
        	//usleep(30000);
        	SDLClear();//LIMPIO PANTALLA
	       	
        	//CONTADOR DE NAVES ENEMIGAS PARA REGENERAR
        	for(i=0;i<cant_filas;i++)
        		for(j=0;j<cant_naves;j++)
        			navesActivas+=naves[i][j].visible;//contador de naves eliminadas, cuando no queden naves, las regenera
		
        	if(navesActivas==0)//SI LA MATRIZ SE TERMINO LAS REINICIA
        	  	iniciarNaves(cant_naves,cant_filas,naves,150);
		
        	moverNaves(cant_filas,cant_naves,naves);//Mover naves extraterrestes

		//CALCULA EXPLOSION
		if(contExplo>=3){
			setPosition(&explosion, 900,900);
			contExplo=0;
		}
		else{
			addScreen(&explosion, BOTH);
			contExplo++;
		}
		
		int h;
		int cant=150;
		
		//DIBUJA LAS VIDAS EN LA PANTALLA		
		for(h=0;h<frameNave.vidas;h++){
			setPosition(&frameVida,MARGEN_X-cant,MARGEN_Y-30);
			addScreen(&frameVida,BOTH);
			cant-=50;
		}

		//NAVE NEUTRAL
		if(naveRoja.visible==1){
			if(naveRoja.position.x<=0)//Cuando se sale del marco, vuelve a empezar desde la derecha
				setPosition(&naveRoja, MARGEN_X -220, 300);
			else{
				addScreen(&naveRoja, BOTH);
				setPosition(&naveRoja, (naveRoja.position.x) -4 , naveRoja.position.y);
			}
		}					
                
                //AGREGA LA EXPLOSION
		addScreen(&explosion, BOTH);
        
		//// PEGO EL FONDO SOBRE EL BUFFER ////
		addScreen(NULL,DEFAULT);

	        //MOSTRANDO ESCUDOS DE ABAJO
		for(p=0;p<12;p++){
		        if(bothShield1.bloques[p].visible==1)
			        addScreen(&bothShield1.bloques[p],BOTH);
			if(bothShield2.bloques[p].visible==1)
				addScreen(&bothShield2.bloques[p],BOTH);
			if(bothShield3.bloques[p].visible==1)
				addScreen(&bothShield3.bloques[p],BOTH);
			if(bothShield4.bloques[p].visible==1)
				addScreen(&bothShield4.bloques[p],BOTH);
	   	}
	   	//MOSTRANDO ESCUDOS DE ARRIBA
		for(t=0;t<12;t++){                       
			if(topShield1.bloques[t].visible==1)
				addScreen(&topShield1.bloques[t],BOTH);
			if(topShield2.bloques[t].visible==1)
				addScreen(&topShield2.bloques[t],BOTH);
			if(topShield3.bloques[t].visible==1)
				addScreen(&topShield3.bloques[t],BOTH);
			if(topShield4.bloques[t].visible==1)
				addScreen(&topShield4.bloques[t],BOTH);
	        }
		//MOSTRAR MATRIZ MARCIANA
		for(i=0;i<cant_filas;i++)
			for(j=0;j<cant_naves;j++)
				if(naves[i][j].visible==1)
					addScreen(&naves[i][j],BOTH);
		//MOSTRANDO NAVE PLAYER 1
		if(frameNave.visible==1){
		        addScreen(&frameNave,BOTH);
		}else 
			if(frameNave.visible==0 && frameNave.vidas>0){
				frameNave.visible=1;
				setPosition(&frameNave,(MARGEN_X/2)-72,MARGEN_Y-30);
			}		              
		//SI SE QUEDA SIN VIDAS PIERDE
		if(frameNave.vidas == 0)
			terminar=1;

                //BALA DE MARCIANOS
                srandom(time(NULL) + (i*4));
                int numerito = rand() % 3;//Despues de un tiempo aleatorio, disparan balas las naves extraterrestres    
		for(i=0;i<cant_filas;i++)
			for(j=0;j<cant_naves;j++)	
				if(naves[i][j].visible==1 && bala_activaEnemiga==0 && numerito==1){	
					bala_activaEnemiga=1;					
                                        setPosition(&frameBalaEnemiga,(naves[i][j]).position.x+3,
                                        (naves[i][j]).position.y+3);										
				}
                //// SI TENGO UNA BALA ACTIVA extraterreste LA MUEVE HACIA abajo, SI LLEGO AL fondo DESAPARECE ////
		if(bala_activaEnemiga==1){
			//// CAMBIO LA POSICION VERTICAL DE LA BALA HACIA ARRIBA ////
			frameBalaEnemiga.position.y+=3;
			
			//// PEGO LA BALA SOBRE EL BUFFER ////
			addScreen(&frameBalaEnemiga,BOTH);

			if(verificarColision3(&frameBalaEnemiga, &frameNave) ==0){
				frameNave.visible=0;
				bala_activaEnemiga=0;
				frameNave.vidas--;//pierde una vida
			}
					
			if(verificarShield(&frameBalaEnemiga,bothShield1.bloques)==0 ||
			   verificarShield(&frameBalaEnemiga,bothShield2.bloques)==0 ||
		           verificarShield(&frameBalaEnemiga,bothShield3.bloques)==0 ||
			   verificarShield(&frameBalaEnemiga,bothShield4.bloques)==0)
			        bala_activaEnemiga=0;
				 
			if(verificarShield(&frameBalaEnemiga,topShield1.bloques)==0 ||
			   verificarShield(&frameBalaEnemiga,topShield2.bloques)==0 ||
			   verificarShield(&frameBalaEnemiga,topShield3.bloques)==0 ||
			   verificarShield(&frameBalaEnemiga,topShield4.bloques)==0)
			        bala_activaEnemiga=0;					

			//// SI LA BALA LLEGO AL FONDO DEBE DESAPARECER ////
			if(frameBalaEnemiga.position.y>=MARGEN_Y)
				bala_activaEnemiga=0;		 
		}
		//// SI TENGO UNA BALA ACTIVA LA MUEVE HACIA ARRIBA, SI LLEGO AL TOPE DESAPARECE ////
		if(bala_activa==1){
		        //// CAMBIO LA POSICION VERTICAL DE LA BALA HACIA ARRIBA ////
			frameBala.position.y-=5;
			//// PEGO LA BALA SOBRE EL BUFFER ////
			addScreen(&frameBala,BOTH);
        		//Colision con los extraterrestres
			if(verificarColision(&frameBala, naves,cant_filas, cant_naves, &explosion)==0){
				bala_activa=0;
				puntaje+=10;
				contExplo=0;
			} 
			//Colision con la nave roja
			if( verificarColision2(&frameBala, &naveRoja, &explosion) == 0){ 
				bala_activa=0;
				puntaje+=30;
				contExplo=0;
			}	 
			//VERIFICO COLISION DE LA BALA CON LOS 4 ESCUDOS DE ABAJO
			if(verificarShield(&frameBala,bothShield1.bloques)==0 ||
			   verificarShield(&frameBala,bothShield2.bloques)==0 ||
			   verificarShield(&frameBala,bothShield3.bloques)==0 || 
			   verificarShield(&frameBala,bothShield4.bloques)==0)
				bala_activa=0;
			//VERIFICO COLISION DE LA BALA CON LOS 4 ESCUDOS DE ARRIBA	
			if(verificarShield(&frameBala,topShield1.bloques)==0 ||
			   verificarShield(&frameBala,topShield2.bloques)==0 ||
			   verificarShield(&frameBala,topShield3.bloques)==0 ||
			   verificarShield(&frameBala,topShield4.bloques)==0)
				bala_activa=0;
                        //SI LA BALA LLEGO AL TOPE DEBE DESAPARECER ////
			if(frameBala.position.y<=0)
			        bala_activa=0;                     
		}           
                //ESCRIBE TEXTOS DE TTF
		addText("Jugador 2:",710,20);
		addText("Puntos",710,40);
		addText("Vidas:",710,60);
		addText("Jugador 1:",710,235);
		addText("Puntos",710,255);
		addText("Vidas:",710,275);
		
		//VUELCO TODO EL BUFFER A PANTALLA
		showScreen();
                endTimer = SDL_GetTicks();
   
                deltaTime = endTimer-startTimer;
                if(deltaTime<(1000/FPS))
                        SDL_Delay((1000/FPS)-deltaTime);
    
        }//Termina gameloop

        SDL_Quit();
        pthread_exit(salir);//salgo del thread
}
