#include "_validations.h"

/*	Funcion encagarda de validar que una cadena de caracteres contiene solo valores*/
int es_numero(const char* cadena)
{

    /*verifico que la cadena pueda convertirse a entero*/
    if(atoi(cadena) == 0)
        return 0; //error!!!

    /* Verificamos que todos los caracteres sean un numero */
    while(cadena && *cadena) //mientras la cadena tenga valores me voy moviendo en la cadena referencia por memoria (puntero)
    {
    	if(*cadena >= '0' && *cadena <= '9')
			cadena++;
		else
			return 0;//error!!!
    }

    return 1; //sin error!!!!!!
}

/*Funcion encagarda de validar que una cadena de caracteres contiene solo valores numericos OPCION 2.*/
int validaNumero(char*string)
{
        char *aux=string;aux+=strlen(string)-1;*aux='\0';//ELIMINO EL SALTO DE LINEA '\n'
        regex_t retmp;
        regmatch_t mtmp;
        regcomp(&retmp,"^[0-9]$",REG_EXTENDED);
        if(!regexec(&retmp,string, (size_t) 1, &mtmp, 0))
                return 0;
        return -1;
}

/* Funcion encagarda de validar que un path ingresado sea valido, y si existe y tenga permisos (DIRECTORIO) de LECTURA */
int validaArchPermExit (const char* path)
{
	struct stat datosFichero;
	int salida = 0;
	//lstat permite obtener información de un fichero: propietario, tamaño, permisos, tipo de fichero, etc
	if (lstat(path, &datosFichero) != -1)
	{
		//si el usuario que ejecuta es OWN igual verifica
		if((long) datosFichero.st_uid == (long) getuid())
		{
			if(datosFichero.st_mode & S_IRUSR)//tiene que tener permisos de lectura
				salida = 1;
		}
		else //sino es el own se tendra que fijar que permisos tiene el "OTHER"
		{
			if(datosFichero.st_mode & S_IROTH)//tiene que tener permisos de lectura
				salida = 1;
		}
	}
	return salida;
}

/*Valida si un archivo tiene "algo" sino es vacio*/
int archVacio(const char* path)
{
	struct stat dat;
	int salida = 0;

	if(stat(path,&dat) == -1)
		return(-1);

	if(dat.st_size > 0)
		salida = 1;

	return salida;
}

/*Se obtiene la cantidad de líneas del archivo*/
int getCantLineas (char * NOM_ARCHIVO)
{
	int nLineas = 0;
    FILE *archivo;
    char string[200];
    archivo = fopen(NOM_ARCHIVO, "r");
    while(fgets(string,200,archivo))
    	nLineas++;
    fclose(archivo);
    return nLineas;
}

/*valida si una cadena cumple con un patron determinado que se pasa por parametro*/
int validarLinea(char *string, const char* expresion)
{
	regex_t retmp;
    regmatch_t mtmp;
    int res;
    res = regcomp(&retmp, expresion ,REG_EXTENDED);
    if(res!=0)
       printf("Error al compilar la RegExp (Resultado: %d)\n",res);

    res = regexec(&retmp, string, MAX_MATCHES, &mtmp, 0);//si es 0 el patron esta en la cadena

    regfree(&retmp);

    return res;
}

/*Funcion encagarda de obtener los parametros mediante substring*/
char* getParamString (char* linea)
{
	 int ch = '=';
	 size_t len;
	 char *pdest;
	 char *inpfile = NULL;
	 //buscara la ultima ocurrencia del caracter /
	 pdest = strrchr(linea, ch);
	 //si el caracter separador no esta en la cadena
	 if(pdest == NULL)
		 inpfile = "";//NO TIENE EXTESION
	 else
	 {
		 pdest++; //Apunto al proximo caracter luego de la ultima /.
		 len = strlen(pdest);
		 inpfile = (char*)malloc(len+1);// Crea espacio dinamicamente.
		 strncpy(inpfile, pdest, len+1);
	 }
	 return inpfile;
}

/*Funcion que valida el formato del archivo*/
char* validaIP(char *linea)
{
	char* salida = NULL;
	const char* IPserver = "^IP_nombre_del_server=(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])[.]){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$";
	const char* nameserverNorma = "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9[-]]*[a-zA-Z0-9])[.])*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9[-]]*[A-Za-z0-9])$";//NORMA RFC 1123
	const char* nameserver = "^IP_nombre_del_server=[a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]+$";

	if(validarLinea(linea,IPserver)==0)
		salida = getParamString(linea);
	else if (validarLinea(linea,nameserver)==0 || validarLinea(linea,nameserverNorma)==0)
		salida = getParamString(linea);
	else
		printf("ERROR: Formato erroneo en linea de configuracion de IP ó nombre del Server.\nAsegurese que la ip sea del tipo: 255.255.255.255 o directamente coloque el nombre descriptivo del server en la red (DNS)\n");

	return salida;
}

/*Funcion que valida el formato del archivo*/
int validaPuerto(char *linea)
{
	int puerto = 0, salida = 0;
	const char* parametroName = "^puerto=[0-9][0-9][0-9][0-9][0-9]$";//que contenga puerto= y que lo que sigue sean 5 numeros

	if(validarLinea(linea,parametroName)==0)
	{
		puerto = atoi(getParamString(linea));
		if (puerto <= 49152 || puerto >= 65535)
			printf("ERROR: El parámetro ingresado debe ser un puerto del rango 49152-65535. Chequee ademas que sea el Puerto que conecta al server.\n");
		else
			salida = puerto;
	}
	else
		printf("ERROR: Formato erroneo en linea de configuracion de puerto.\n");

	return salida;
}

/*Funcion para obtener tecla en SDL*/
SDLKey configurarTecla(char* teclaDesc)
{
	int num;
	SDLKey tecla;
	if(strcmp(teclaDesc,"flecha derecha")==0)num=0;
	else if(strcmp(teclaDesc,"flecha izquierda")==0)num=1;
	else if(strcmp(teclaDesc,"q")==0)num=2;
	else if(strcmp(teclaDesc,"w")==0)num=3;
	else if(strcmp(teclaDesc,"e")==0)num=4;
	else if(strcmp(teclaDesc,"r")==0)num=5;
	else if(strcmp(teclaDesc,"t")==0)num=6;
	else if(strcmp(teclaDesc,"y")==0)num=7;
	else if(strcmp(teclaDesc,"u")==0)num=8;
	else if(strcmp(teclaDesc,"i")==0)num=9;
	else if(strcmp(teclaDesc,"o")==0)num=10;
	else if(strcmp(teclaDesc,"p")==0)num=11;
	else if(strcmp(teclaDesc,"a")==0)num=12;
	else if(strcmp(teclaDesc,"s")==0)num=13;
	else if(strcmp(teclaDesc,"d")==0)num=14;
	else if(strcmp(teclaDesc,"f")==0)num=15;
	else if(strcmp(teclaDesc,"g")==0)num=16;
	else if(strcmp(teclaDesc,"h")==0)num=17;
	else if(strcmp(teclaDesc,"j")==0)num=18;
	else if(strcmp(teclaDesc,"k")==0)num=19;
	else if(strcmp(teclaDesc,"l")==0)num=20;
	else if(strcmp(teclaDesc,"z")==0)num=21;
	else if(strcmp(teclaDesc,"x")==0)num=22;
	else if(strcmp(teclaDesc,"c")==0)num=23;
	else if(strcmp(teclaDesc,"v")==0)num=24;
	else if(strcmp(teclaDesc,"b")==0)num=25;
	else if(strcmp(teclaDesc,"n")==0)num=26;
	else if(strcmp(teclaDesc,"m")==0)num=27;
	else if(strcmp(teclaDesc,"espacio")==0)num=28;
	else if(strcmp(teclaDesc,"flecha arriba")==0)num=30;
	switch(num){
		case 0:tecla = SDLK_RIGHT;break;
		case 1:tecla = SDLK_LEFT;break;
		case 2:tecla = SDLK_q;break;
		case 3:tecla = SDLK_w;break;
		case 4:tecla = SDLK_e;break;
		case 5:tecla = SDLK_r;break;
		case 6:tecla = SDLK_t;break;
		case 7:tecla = SDLK_y;break;
		case 8:tecla = SDLK_u;break;
		case 9:tecla = SDLK_i;break;
		case 10:tecla = SDLK_o;break;
		case 11:tecla = SDLK_p;break;
		case 12:tecla = SDLK_a;break;
		case 13:tecla = SDLK_s;break;
		case 14:tecla = SDLK_d;break;
		case 15:tecla = SDLK_f;break;
		case 16:tecla = SDLK_g;break;
		case 17:tecla = SDLK_h;break;
		case 18:tecla = SDLK_j;break;
		case 19:tecla = SDLK_k;break;
		case 20:tecla = SDLK_l;break;
		case 21:tecla = SDLK_z;break;
		case 22:tecla = SDLK_x;break;
		case 23:tecla = SDLK_c;break;
		case 24:tecla = SDLK_v;break;
		case 25:tecla = SDLK_b;break;
		case 26:tecla = SDLK_n;break;
		case 27:tecla = SDLK_m;break;
		case 28:tecla = SDLK_SPACE;break;
		case 30:tecla = SDLK_UP;break;
		default:break;
	}

	return tecla;
}

/*Funcion que valida el formato del archivo*/
char* validaTeclas(char *linea, int validatecla)
{
	char* salida = NULL;
	const char* parametroName;
	switch(validatecla){
			case 1:parametroName = "^tecla_derecha=[a-z]$|^tecla_derecha=flecha derecha$|^tecla_derecha=flecha izquierda$|^tecla_derecha=flecha arriba$|^tecla_derecha=espacio$";break;
			case 2:parametroName = "^tecla_izquierda=[a-z]$|^tecla_izquierda=flecha derecha$|^tecla_izquierda=flecha izquierda$|^tecla_izquierda=flecha arriba$|^tecla_izquierda=espacio$";break;
			case 3:parametroName = "^tecla_disparo=[a-z]$|^tecla_disparo=flecha derecha$|^tecla_disparo=flecha izquierda$|^tecla_disparo=flecha arriba$|^tecla_disparo=espacio$";break;
			default:break;
	}

	if(validarLinea(linea,parametroName)==0)
		salida = getParamString(linea);
	else
	{
		switch(validatecla){
			case 1:printf("ERROR: Formato erroneo en linea de configuracion de tecla derecha.\n");break;
			case 2:printf("ERROR: Formato erroneo en linea de configuracion de tecla izquierda.\n");break;
			case 3:printf("ERROR: Formato erroneo en linea de configuracion de tecla disparo.\n");break;
			default:break;
		}
	}
	return salida;
}

/*valida la solo existencia de un archivo*/
int existArch(const char* path)
{
struct stat datosFichero;
	int salida = 0;

	//lstat permite obtener información de un fichero: propietario, tamaño, permisos, tipo de fichero, etc
	if (lstat(path, &datosFichero) != -1)
		salida = 1;//existe!

	return salida;
}

/*quita los espacios de una cadena*/
char *trim(char *str)
{
      char *ibuf = str, *obuf = str;
      int i = 0, cnt = 0;
      if (str)
      {
            for (ibuf = str; *ibuf && isspace(*ibuf); ++ibuf);
            if (str != ibuf)
                  memmove(str, ibuf, ibuf - str);

            while (*ibuf)
            {
                  if (isspace(*ibuf) && cnt)
                        ibuf++;
                  else
                  {
                        if (!isspace(*ibuf))
                              cnt = 0;
                        else
                        {
                              *ibuf = ' ';
                              cnt = 1;
                        }
                        obuf[i++] = *ibuf++;
                  }
            }
            obuf[i] = '\0';
            while (--i >= 0)
            {
                  if (!isspace(obuf[i]))
                        break;
            }
            obuf[++i] = '\0';
      }
      return str;
}

/*obtener un string dinamico a partir de un buffer estatico*/
char *getDinamicString (char staticstring [])
{
	size_t len = strlen(staticstring);//LO MALO DE TENER EL ARRAY DE LA ESTRUCTURA ESTATICO
	char * aux = (char*)malloc(len+1);
	strncpy(aux, staticstring, len+1);
	return aux;
}

//valida el formato del archivo en general y carga los parametros en las variables
int validaFormatoYcarga (char * path)
{
	int TAMBUF = 512;
	int salida = 1,cantLineas = 1,numero = 0;
	char *param;
	FILE *archivo;
	char linea[TAMBUF];
	char* string;
	archivo = fopen(path,"r");

	while(fgets(linea,TAMBUF,archivo) && salida == 1)//si ya encontro algun formateo erroneo no entra
	{
		string = trim(getDinamicString(linea));//me aseguro que el dato se integro sin blancos, espacios, etc
		switch(cantLineas)//mientras las n primeras cumplan con el formato y tengan los datos bien si tiene mas lineas no importaria
		{
			case 1:
				param = validaIP(string);
				if (param != NULL)
					Param.IP_DNS = param;
				else
					salida = 0;
				break;

			case 2:
				numero = validaPuerto(string);
				if (numero != 0)
					Param.puerto = numero;
				else
					salida = 0;
				break;

			case 3:
				param = validaTeclas(string, 1);
				if (param != NULL)
					Param.tecla_derecha = configurarTecla(param);
				else
					salida = 0;
				break;

			case 4:
				param = validaTeclas(string, 2);
				if (param != NULL)
					Param.tecla_izquierda = configurarTecla(param);
				else
					salida = 0;
				break;

			case 5:
				param = validaTeclas(string, 3);
				if (param != NULL)
					Param.tecla_disparo = configurarTecla(param);
				else
					salida = 0;
				break;

			default:
				break;

		}
		cantLineas++;
	}
	fclose(archivo);
	return salida;
}

/*Imprime el mensaje generico para la salida erronea*/
void mensajeSalida()
{
	printf("Formato del archivo y lo que debe contener: \n");
	printf("IP_nombre_del_server=000.000.000.000 ó NombreDNS\n");
	printf("puerto=numero Natural mayor a cero del rango 49152-65535\n");
	printf("tecla_derecha=nombre_tecla\n");
	printf("tecla_izquierda=nombre_tecla\n");
	printf("tecla_disparo=nombre_tecla\n");
	printf("Para más ayuda revise el manual del juego\n");
}

/*FUNCION LLAMADORA DESDE EL MAIN*/
int validacionGeneral(int argc, char *argv[])
{
	int salida = 0;
	char* pathArchConfig = "./config/config_cliente.ini";
	/*******VALIDACIONES******/
	//controlo si la cantidad de parametros ingresados en la correcta
	if (argc > 1)//solo se debe ejecutar con el nombre del ejecutable
	{
		printf("ERROR: La cantidad de Parametros ingresados no es correcta.\n");
		printf("Uso: %s n", argv[0]);
		salida = EXIT_FAILURE;
	}

	//QUE EL ARCHIVO EXISTA Y TENGA PERMISOS DE LECTURA
	else if(validaArchPermExit(pathArchConfig) == 0)
	{
		printf("El archivo de configuracion no es un archivo valido o no existe o no cuenta con permisos de lectura. \nEl archivo debe encontrarse en el siguiente path: %s\n\n", pathArchConfig);
		mensajeSalida();
		salida = EXIT_FAILURE;
	}

	//Valido que el archivo tenga algo
	else if (archVacio(pathArchConfig) == 0)
	{
		printf("El archivo ingresado esta Vacio \n\n");
		mensajeSalida();
		salida = EXIT_FAILURE;
	}

	//valido formato la cantidad de lineas deben ser 5 y ademas estar en correcto formato
	else if (getCantLineas (pathArchConfig) != 5 || validaFormatoYcarga(pathArchConfig) == 0)
	{
		printf("El archivo de configuracion no cumple con las especificaciones del formato.\n\n");
		mensajeSalida();
		salida = EXIT_FAILURE;
	}

	/********FIN VALIDACIONES********/
	return salida;
}


