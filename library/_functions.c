#include"_functions.h"
//Inicia los escudos
void iniciarEscudos1(FrameShield*frame,int mode,int variable){
int yy;

        for(yy=0;yy<12;yy++)
                frame->bloques[yy].visible=1;
        int x=200+variable,y=335;
        int xx=0;//51
        setBox(&frame->bloques[0],xx,21,17,19);
        setPosition(&frame->bloques[0],x,y);
        
        setBox(&frame->bloques[1],xx+17,21,16,10);
        setPosition(&frame->bloques[1],x+17,y);
        
        setBox(&frame->bloques[2],xx+33,21,17,19);
        setPosition(&frame->bloques[2],x+33,y);
        
        setBox(&frame->bloques[3],xx+17,31,16,9);
        setPosition(&frame->bloques[3],x+17,y+10);
        //////////////////////////////////////
        setBox(&frame->bloques[4],xx,40,17,12);
        setPosition(&frame->bloques[4],x,y+19);
        
        setBox(&frame->bloques[5],17+xx,40,16,12);
        setPosition(&frame->bloques[5],x+17,y+19);
        
        setBox(&frame->bloques[6],33+xx,40,17,12);
        setPosition(&frame->bloques[6],x+33,y+19);
        //////////////////////////////////////
        setBox(&frame->bloques[7],xx,52,17,12);
        setPosition(&frame->bloques[7],x,y+31);
        
        setBox(&frame->bloques[8],17+xx,52,16,12);
        setPosition(&frame->bloques[8],x+17,y+31);
        
        setBox(&frame->bloques[9],33+xx,52,17,12);
        setPosition(&frame->bloques[9],x+33,y+31);
        ///////////////////////////////////////
        setBox(&frame->bloques[10],xx,64,18,10);
        setPosition(&frame->bloques[10],x,y+43);
        
        setBox(&frame->bloques[11],32+xx,64,18,10);
        setPosition(&frame->bloques[11],x+32,y+43);
}
//Iniciaria los escudos del rival
void iniciarEscudos2(FrameShield*frame,int mode,int variable){
        int i;
        for(i=0;i<12;i++)
                frame->bloques[i].visible=1;
        int x=200+variable,y=50;
        int xx=103;//51

        setBox(&frame->bloques[0],xx,21,17,19);
        setPosition(&frame->bloques[0],x,y);
        
        setBox(&frame->bloques[1],xx+17,21,16,10);
        setPosition(&frame->bloques[1],x+17,y);
        
        setBox(&frame->bloques[2],xx+33,21,17,19);
        setPosition(&frame->bloques[2],x+33,y);
        
        setBox(&frame->bloques[3],xx+17,64,16,9);
        setPosition(&frame->bloques[3],x+17,y+43);
        //////////////////////////////////////
        setBox(&frame->bloques[4],xx,40,17,12);
        setPosition(&frame->bloques[4],x,y+19);
        
        setBox(&frame->bloques[5],17+xx,40,16,12);
        setPosition(&frame->bloques[5],x+17,y+19);
        
        setBox(&frame->bloques[6],33+xx,40,17,12);
        setPosition(&frame->bloques[6],x+33,y+19);
        //////////////////////////////////////
        setBox(&frame->bloques[7],xx,52,17,12);
        setPosition(&frame->bloques[7],x,y+31);
        
        setBox(&frame->bloques[8],17+xx,52,16,12);
        setPosition(&frame->bloques[8],x+17,y+31);
        
        setBox(&frame->bloques[9],33+xx,52,17,12);
        setPosition(&frame->bloques[9],x+33,y+31);
        ///////////////////////////////////////
        setBox(&frame->bloques[10],xx,64,18,10);
        setPosition(&frame->bloques[10],x,y+43);
        
        setBox(&frame->bloques[11],32+xx,64,18,10);
        setPosition(&frame->bloques[11],x+32,y+43);
}

void startMonitor(){
        pid_t proceso;
        if((proceso=fork())==-1){
                printf("Error\n");
                exit (1);
        }

        if(proceso==0){
                int bandera=1;
	        while(bandera==1){
	        	sleep(3);	
	        	if(getppid() == 1)
	        	bandera=0;
	        }
                printf("Liberando recursos...\n");
                Send("exit");
                sleep(2);
		close(caller_socket);
                exit (1);		
        }
}
//Funcion del handler de la señal
void esperar(int a){
        int proceso;
        wait(NULL);
        startMonitor();
}

void setSignal(){
        struct sigaction aesperar;
        aesperar.sa_handler= esperar;
        aesperar.sa_flags=0;
        sigemptyset(&aesperar.sa_mask);
        sigaction(SIGCHLD, &aesperar, NULL);    
}
/*
void setConfigKey(int *port,char*ip,SDLKey *derecha,SDLKey *izquierda,SDLKey *disparo){
        FILE *pf;char leer[200];
	pf=fopen("./configuracion","rb"); 
	if(pf==NULL){
	  printf("Error en la apertura del archivo de configuracion\n");
	  exit(1);
	}
	fgets(leer,sizeof(leer),pf);
	strcpy(ip,leer);
	//strncpy((*ip+strlen(*ip)-1),"",1);
	fgets(leer,sizeof(leer),pf);
	*port=atoi(leer);
	fgets(leer,sizeof(leer),pf);
	configurarTecla(derecha,leer);
	fgets(leer,sizeof(leer),pf);
	configurarTecla(izquierda,leer);
	fgets(leer,sizeof(leer),pf);
	configurarTecla(disparo,leer);
	//CIERRO EL ARCHIVO PARA QUE OTRO LO PUEDA LEER
	fclose(pf);	
}
*/
void check_mouse_position(Uint16 x, Uint16 y, int *status_exit, int *status_stop){
        if(*status_stop==0){//SI NO ESTA EN FOCUS
                if((x>308 && x<557) && (y>142 && y<164)){//SI ESTOY DENTRO
                        setSprite(&btn_exit,BTN_EXIT_FOCUS);//SETEO IMAGEN DEL BOTON TERMINAR
                        *status_stop=1;//LO PONGO EN FOCO
			pthread_mutex_unlock(&mutexSDL);
                }//SI ESTOY FUERA NO HAGO NADA
        }else{//SI ESTA EN FOCUS
                if(!((x>308 && x<557) && (y>142 && y<164))){//SI NO ESTOY DENTRO
                        setSprite(&btn_exit,BTN_EXIT);//SETEO IMAGEN DEL BOTON TERMINAR
                        *status_stop=0;//LO PONGO FUERA DE FOCO
                        pthread_mutex_unlock(&mutexSDL);
                }//SI ESTOY DENTRO NO HAGO NADA
        }
        if(*status_exit==0){//SI NO ESTA EN FOCUS
                if((x>308 && x<557) && (y>92 && y<115)){//SI ESTOY DENTRO
                        setSprite(&btn_stop,BTN_STOP_FOCUS);//SETEO IMAGEN DEL BOTON EXIT
                        *status_exit=1;//LO PONGO EN FOCO
                        pthread_mutex_unlock(&mutexSDL);
                }//SI ESTOY FUERA NO HAGO NADA
        }else{//SI ESTA EN FOCUS
                if(!((x>308 && x<557) && (y>92 && y<115))){//SI NO ESTOY DENTRO
                        setSprite(&btn_stop,BTN_STOP);//SETEO IMAGEN DEL BOTON EXIT
                        *status_exit=0;//LO PONGO FUERA DE FOCO
                        pthread_mutex_unlock(&mutexSDL);
                }//SI ESTOY DENTRO NO HAGO NADA
        }
}

//Verificar si cuando hay coalision, los dos frames estan incluidos
int include(Frame*f1,Frame*f2){
        if(f1->position.x > f2->position.x && 
           f1->position.x < f2->position.x + f2->box.w && 
           f1->position.y > f2->position.y && 
           f1->position.y < f2->position.y + f2->box.h)                        
                return 1;
        return 0;
}
//VERIFICA LA COALISION DE NUESTRO DISPARO CON LOS ENEMIGOS
int verificarColision(Frame *frameBala, Frame naves[5][10],int cant_fil, int cant_col, Frame *explo){
        int i,j;
        for(i=0;i<cant_fil;i++)
        	for(j=0;j<cant_col;j++)
			if(include(frameBala,&naves[i][j])==1 && naves[i][j].visible==1){
				setPosition(explo, naves[i][j].position.x, naves[i][j].position.y);
				naves[i][j].visible = 0;
				return 0;
			}
        return 1;
}
//VERIFICA LA COALISION DE NUESTRO DISPARO CON LA NAVE ROJA
int verificarColision2(Frame *frameBala, Frame *naveRoja, Frame *explo){
        if(include(frameBala,naveRoja)==1 && (*naveRoja).visible ==1){
                naveRoja->visible=0;
		setPosition(explo,naveRoja->position.x,naveRoja->position.y); 
		setPosition(naveRoja,1700,naveRoja->position.y);//MOVER HACIA LA IZQUIERDA
		return 0;
        }
        return 1;
}
//VERIFICA LA COALISION DE EL DISPARO ENEMIGO CON LA NAVE
int verificarColision3(Frame *frameBalaEnemiga, Frame *frameNave){
        if(include(frameBalaEnemiga,frameNave)==1 && frameNave->visible ==1){
                frameNave->visible = 0;
                setPosition(frameNave,1700,frameNave->position.y);//MOVER HACIA LA IZQUIERDA
                return 0;
        }
        return 1;
}

int verificarShield(Frame *bala,Frame *frame){
        int i;
        for(i=0;i<12;i++){
                if(include(bala,&frame[i])==1 && frame[i].visible==1){
                        frame[i].visible=0;
                        return 0;
                }
        }
        return 1;
}

//MUEVE LAS NAVES DE UN LADO AL OTRO Y ALTERNA LAS IMAGENES EN CADA MOVIMIENTO
void moverNaves(int cant_filas,int cant, Frame naves[5][10]){
	int x,y,i,j;
        if(naves[0][cant-1].position.x>MARGEN_X-220)mult=-1;//MOVER HACIA LA IZQUIERDA
        if(naves[0][0].position.x<50)mult=1;//MOVER HACIA LA DERECHA
        if(status_>400)status_=0;//INTERVALO DE ALTERNANCIA
	 
        for(i=0;i<cant_filas;i++)
        	for(j=0;j<cant;j++)//PROCESO EL ARRAY DE NAVES
               	 	if(++status_<200){
			        if(i==0 || i==4)
                        	        setSprite(&naves[i][j],ENEMY_C);
                                else
                                        setSprite(&naves[i][j],ENEMY_A);
                       	        if(status_==199)
					for(x=0;x<cant_filas;x++)
						for(y=0;y<cant;y++)
							naves[x][y].position.x+=10*mult;
                	}
               		 else{
               		        if(i==0 || i==4)
          	                        setSprite(&naves[i][j],ENEMY_D);
                                else
                                        setSprite(&naves[i][j],ENEMY_B);
          	                if(status_==400)
			    		for(x=0;x<cant_filas;x++)
						for(y=0;y<cant;y++)
          	                            		naves[x][y].position.x+=10*mult;
	                }	        
	//FIN FOR
}
//Inicia las naves extraterrestres
void iniciarNaves(int cant_col,int cant_filas, Frame naves[5][10], int y){
        int i,j;
        for(i=0;i<cant_filas;i++)
                for(j=0;j<cant_col;j++){
			setPosition(&(naves[i][j]), 100+(j*30) , y+(i*30));
			naves[i][j].visible=1;
                        setSprite(&naves[i][j],ENEMY_A);
		}
}

void iniciarEscudos(){
        iniciarEscudos1(&bothShield1,0,0);
        iniciarEscudos1(&bothShield2,0,100);
        iniciarEscudos1(&bothShield3,0,200);
        iniciarEscudos1(&bothShield4,0,300);
        iniciarEscudos2(&topShield1,0,0);
        iniciarEscudos2(&topShield2,0,100);
        iniciarEscudos2(&topShield3,0,200);
        iniciarEscudos2(&topShield4,0,300);
}

void Send(char* msg){
        strcpy(buffer,msg);
        pthread_mutex_unlock(&mutex);//SEND
}
