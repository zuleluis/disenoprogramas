/*
Hacer un programa en C que de modo gráfico usando OpenGL:
    - Genere y muestre un grafo conectado y dirigido, basado en las palabras del archivo.
    - Guarde el grafo y que pueda anexar otro archivo de texto al grafo, actualizando el 
       grafo existente y sin borrar los datos previos.
    - Permita capturar en tiempo de ejecución una frase y que palabra por palabra haga la predicción de 
      la palabra siguiente basándose en el camino más corto (Dijkstra) desde la primera palabra capturada,
      una vez capturada debe actualizar el grafo. (2 PTS)
    - Tenga una opción para desplegar las frases más comunes ordenadas de la más común a la menos, dada una
    palabra inicial por el usuario. (2 PTS)
    - Debe mostrar las palabras y las frases en un color distinto.
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

struct nodo{
    char palabra[30];
    int x;
    int y;
    float ocurrencia; //Guarda la probabilidad de ocurrencia P(A)=d/m
    int frecuencia; //Guarda la frecuencia con la que aparece cada palabra
    int frecuencia_aristas; //Guarda la cantidad de veces que aparece la palabra en una arista
    struct nodo *siguiente;
    struct arista *aristas;
};

struct arista{
    float peso;
    int frecuencia; //Cantidad de veces que se repite la misma arista
    struct nodo *vertice;
    struct arista *siguiente;
};

//Funciones de grafos
int inserta_nodo(struct nodo **grafo, char *palabra);
struct nodo *busca_nodo(struct nodo *grafo, char *nodo_buscado);
int inserta_arista(struct nodo *grafo, char *origen, char *destino);
struct arista *busca_arista(struct arista *arista, char *destino);
int imprime_lista(struct nodo *grafo);
int imprime_aristas(struct arista *arista);
int ordena_nodos(struct nodo *grafo);
int ordena_aristas(struct arista **arista, bool *estado); //Funcion para ordenar aristas de la mas lejana a la mas cercana
int frases_comunes(struct nodo *grafo, char *palabra);
int elimina_nodos(struct nodo **grafo);
void elimina_aristas(struct arista **arista);
int cantidad_elementos(struct nodo *grafo);

int lee_archivo(struct nodo **grafo, char *nombre_archivo);
char evalua_caracter_especial(char letra);
int guarda_grafo(struct nodo *grafo, char *nombre_archivo);
int escribe_datos(FILE *archivo, struct nodo *grafo);
int escribe_aristas(FILE *archivo, struct arista *aristas);

int prob_ocurrencia(struct nodo **grafo);
int teorema_bayes(struct nodo *grafo);
int bayes_aristas(struct nodo *anterior, struct arista **arista);

//Funciones de graficos
void inicializa();
static void grafos(void);
void texto(char *frase);
void texto2(char *frase);
static void redimensiona(int ancho, int alto);
void asignar_posiciones(struct nodo *grafo,int posx,int posy,int contador);
void crea_nodo(struct nodo *grafo);
void crea_aristas(struct nodo *grafo);
void une_aristas(struct nodo *grafo, struct arista *arista);
void teclas_direccion(int tecla, int posx, int posy);
void actualizar();
double red(double color);
double green(double color);
double blue(double color);
void menu_principal(int opcion);


//Variables globales
int elementos=0; //Cantidad de elementos en el grafo
int x=100, y=100, rango=0, tam=200;
struct nodo *raiz=NULL;
char frase[50];
int ventana=0,ventana2=0;


int main(int argc, char *argv[])
{
    /*inserta_nodo(&raiz, "hola");
    inserta_nodo(&raiz, "esto");
    inserta_nodo(&raiz, "es");
    inserta_nodo(&raiz, "una");
    inserta_nodo(&raiz, "prueba");
    inserta_nodo(&raiz, "para");
    inserta_nodo(&raiz, "ver");
    inserta_nodo(&raiz, "si");
    inserta_nodo(&raiz, "esto");
    inserta_nodo(&raiz, "funciona");
    inserta_nodo(&raiz, "para");
    inserta_nodo(&raiz, "mi");

    inserta_arista(raiz, "hola", "esto");
    inserta_arista(raiz, "esto", "es");
    inserta_arista(raiz, "es", "una");
    inserta_arista(raiz, "una", "prueba");
    inserta_arista(raiz, "prueba", "para");
    inserta_arista(raiz, "para", "ver");
    inserta_arista(raiz, "ver", "si");
    inserta_arista(raiz, "si", "esto");
    inserta_arista(raiz, "esto", "funciona");
    inserta_arista(raiz, "funciona", "para");
    inserta_arista(raiz, "para", "mi");*/

    /*lee_archivo(&raiz, "prueba2.txt");
    prob_ocurrencia(&raiz);
    teorema_bayes(raiz);
    ordena_nodos(raiz);
    imprime_lista(raiz);
    printf("\n\nHay %i elementos\n", elementos);
    //frases_comunes(raiz, "para");
    if(elimina_nodos(&raiz)==0) elementos=0;
    printf("\n\nHay %i elementos\n", elementos);
    if(imprime_lista(raiz)==-1) printf("Fin de lista\n\n");

    //guarda_grafo(raiz, "backup.txt");*/
    
    glutInit(&argc, argv);
	inicializa();
	glutIdleFunc(actualizar);
	glutMainLoop(); 
    return 0;
}


//Funcion para insertar una nueva palabra al grafo
int inserta_nodo(struct nodo **grafo, char *palabra){
    //Primero, evaluaremos si la palabra a registrar no existe aun en el grafo
    struct nodo *pBusqueda=busca_nodo(*grafo, palabra);

    if(!pBusqueda){ //Si no existe la palabra, creamos una nueva
        struct nodo *pNuevo=(struct nodo*)malloc(sizeof(struct nodo));
        if(!pNuevo) return -1;

        strcpy(pNuevo->palabra, palabra);
        pNuevo->x=0;
        pNuevo->y=0;
        pNuevo->aristas=NULL;
        pNuevo->frecuencia=1;
        pNuevo->frecuencia_aristas=0; //Aun no ha aparecido en ninguna arista
        pNuevo->siguiente=(*grafo);
        *grafo=pNuevo;
    }
    else pBusqueda->frecuencia++; //Si la palabra ya esta registrada, unicamente hay que escribir que aparecio una vez mas
    return 0;
}

struct nodo *busca_nodo(struct nodo *nodo, char *nodo_buscado){
    if(!nodo) return NULL;
    if(strcmp(nodo->palabra,nodo_buscado)==0) return nodo;
    return busca_nodo(nodo->siguiente, nodo_buscado);
}

struct arista *busca_arista(struct arista *arista, char *destino){
    if(!arista) return NULL;
    if(strcmp(arista->vertice->palabra, destino)==0) return arista;
    return busca_arista(arista->siguiente, destino);

}

int inserta_arista(struct nodo *grafo, char *origen, char *destino){
    struct nodo *pOrigen=busca_nodo(grafo, origen); 
    struct nodo *pDestino=busca_nodo(grafo, destino);

    if(!origen || !destino) return -1; //Evaluamos que exista la palabra de origen y la palabra de destino en el grafo
    else{
        struct arista *pArista=busca_arista(pOrigen->aristas, destino); //Comprobamos si existe o no la conexion entre origen y destino
        if(!pArista){
            struct arista *pNuevo=(struct arista*)malloc(sizeof(struct arista));
            if(!pNuevo) return -1;

            pNuevo->frecuencia=1;
            pNuevo->vertice=pDestino;
            pNuevo->siguiente=pOrigen->aristas;
            pOrigen->aristas=pNuevo;
        }
        else pArista->frecuencia++; //Si ya existe, entonces la arista aparece una vez mas
        pOrigen->frecuencia_aristas++; //El origen aparece en una arista mas
    }
    return 0;
}

int imprime_lista(struct nodo *grafo){
    if(!grafo) return -1;
    printf("\n\nVertice -> %s\n", grafo->palabra);
    imprime_aristas(grafo->aristas);
    imprime_lista(grafo->siguiente);
}

int imprime_aristas(struct arista *arista){
    if(!arista) return 0;
    printf("  %16s  %f - %i\n", arista->vertice->palabra, arista->peso, arista->frecuencia);
    imprime_aristas(arista->siguiente);
}

int lee_archivo(struct nodo **grafo, char *nombre_archivo){
    //strcat(nombre_archivo, ".txt");
    
    FILE *archivo;
    archivo=fopen(nombre_archivo, "rb+");
    //archivo=fopen("prueba.txt", "rb+");

    if(!archivo) return -1;
    char letra, auxiliar;

    char palabra[30], origen[30], destino[30];
    int i=0, inicio=0;

    while(!feof(archivo)){ //Leemos letra a letra el archivo
        fread(&letra,sizeof(char),1,archivo);

        if(letra=='.' || letra==',' || letra=='-' || letra=='?' || letra=='!' || letra==-62){
            if(letra==-62) fread(&letra,sizeof(char),1,archivo);
            fread(&letra,sizeof(char),1,archivo);
        }

        if(letra==' ' || letra=='\n'){
            letra='\0';
        }
        
        //Evalua letras con acentos o ~
        if(letra==-61){
            fread(&letra,sizeof(char),1,archivo);
            letra=evalua_caracter_especial(letra);
        }

        //Evaluamos que la letra no este en mayusculas para evitar duplicidad de palabras
        if(letra>=65 && letra<=90) letra=tolower(letra);

        if(letra>=97 && letra<=122){
            palabra[i]=letra;
            i++;
        }
        if(letra=='\0' && palabra[0]!='\0'){
            if(!feof(archivo)){
                palabra[i]='\0';
                i=0;
                elementos++;
                inserta_nodo(grafo, palabra);
                //printf("Palabra: %s\n", palabra);
                if(inicio==0){
                    strcpy(origen, palabra);
                    inicio=1; //Nos ayuda a evaluar que ya empezamos a crear aristas
                }
                else{
                    strcpy(destino, palabra);
                    //printf("Origen: %s\nDestino: %s\n\n", origen, destino);
                    inserta_arista(*grafo, origen, destino);
                    strcpy(origen, destino);
                }
            }
        }
    }
    fclose(archivo);
    return 0;
}

//Convierte caracteres especiales a caracteres comunes
char evalua_caracter_especial(char letra){
    if(letra==-95 || letra==-127) return 'a';
    if(letra==-87 || letra==-119) return 'e';
    if(letra==-83 || letra==-115) return 'i';
    if(letra==-77 || letra==-109) return 'o';
    if(letra==-70 || letra==-102) return 'u';
    if(letra==-79 || letra==-111) return 'n';
    else return 'x';
}

int guarda_grafo(struct nodo *grafo, char *nombre_archivo){
    FILE *archivo;

    archivo=fopen(nombre_archivo, "w+");
    if(!archivo){
        fclose(archivo);
        return -1;
    }

    remove(nombre_archivo); //Borramos archivo para evitar duplicidad de datos
    archivo=fopen(nombre_archivo, "a+"); //Y lo volvemos a abrir
    escribe_datos(archivo, grafo);    

    fclose(archivo);
    return 0;

}

int escribe_datos(FILE *archivo, struct nodo *grafo){
    if(!grafo) return 0;

    fprintf(archivo, "%s\n", grafo->palabra);
    escribe_aristas(archivo, grafo->aristas);
    escribe_datos(archivo, grafo->siguiente);
}

int escribe_aristas(FILE *archivo, struct arista *aristas){
    if(!aristas) return 0;
    fprintf(archivo, "%s\n", aristas->vertice->palabra);
    imprime_aristas(aristas->siguiente);
}

int prob_ocurrencia(struct nodo **grafo){
    if(!*grafo) return 0;
    (*grafo)->ocurrencia=(float)(*grafo)->frecuencia/elementos;
    prob_ocurrencia(&(*grafo)->siguiente);
}

/*TEOREMA DE BAYES
FORMULAS

         P(B|A)*P(A)
P(A|B)=---------------
            P(B)
          
         P(BnA)
P(B|A)= --------- ; P(A)!= 0
          P(A)

=Despejando P(B|A) en P(A|B)...=

           P(BnA) * ~P(A)~
P(A|B)=-------------------
            P(B) * ~P(A)~

Por lo tanto, despues del despeje del Teorema de Bayes

        P(BnA)
P(A|B)=--------
         P(B)


*/

int teorema_bayes(struct nodo *grafo){
    if(!grafo) return 0;
    bayes_aristas(grafo, &grafo->aristas);
    teorema_bayes(grafo->siguiente);
}

int bayes_aristas(struct nodo *anterior, struct arista **arista){
    if(!*arista || !anterior) return 0;

    float BnA=(float)(*arista)->frecuencia/(float)anterior->frecuencia_aristas;
    (*arista)->peso=BnA/anterior->ocurrencia;
    //printf("Peso: %f\n", (*arista)->peso);
    bayes_aristas(anterior, &(*arista)->siguiente);
}

int ordena_nodos(struct nodo *grafo){
	if(!grafo) return 0;
	bool estado=false;
	do{
		estado=false;
		ordena_aristas(&grafo->aristas,&estado);	
	}
    while(estado==true);
	ordena_nodos(grafo->siguiente);
}

int ordena_aristas(struct arista **arista, bool *estado){
	if(!(*arista)){
		return 0;
	}
	if(!(*arista)->siguiente){
		return 0;
	}

	if((*arista)->peso<(*arista)->siguiente->peso){ //Se ordenaran los pesos de las aristas de mayor a menor
		struct arista *pActual=(*arista)->siguiente;
		(*arista)->siguiente=pActual->siguiente;
		pActual->siguiente=(*arista);
		*arista=pActual;
		*estado=true;
	}
	ordena_aristas(&(*arista)->siguiente,estado);
}

int frases_comunes(struct nodo *grafo, char *palabra){
    struct nodo *pBusqueda=busca_nodo(grafo, palabra);
    if(!pBusqueda)return -1;
    
    int contador=0;
    struct arista *pArista=pBusqueda->aristas;

    printf("Frases mas comunes\n");
    printf("%s -> ", pBusqueda->palabra);

    while(pArista){
        while(pArista->vertice){
            printf("%s->", pArista->vertice->palabra);
            pArista->vertice=pArista->vertice->siguiente;
        }
        printf("\n\n\n");
        pArista=pArista->siguiente;
        contador++;
    }
    return 0;
}

int elimina_nodos(struct nodo **grafo){
    if(!*grafo) return 0;
    struct nodo *pAux=*grafo;

    elimina_aristas(&(*grafo)->aristas);

    *grafo=(*grafo)->siguiente;
    free(pAux);
    elimina_nodos(grafo);
}

void elimina_aristas(struct arista **arista){
    if(!*arista) return;
    struct arista *pAux=*arista;

    *arista=(*arista)->siguiente;
    free(pAux);
    elimina_aristas(arista);
}

int cantidad_elementos(struct nodo *grafo){
    if(!grafo){
		rango=(int)sqrt(rango);
		return 0;
	}
	rango++;
	cantidad_elementos(grafo->siguiente);
}

void inicializa(){
    glutInitWindowSize(840,600);
	glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	ventana=glutCreateWindow("EL PREDICTOR");
	glutReshapeFunc(redimensiona);
	glutDisplayFunc(grafos);	
	glClearColor(red(88),green(24),blue(69),1);
    glutSpecialFunc(teclas_direccion);

	glutCreateMenu(menu_principal);
	glutAddMenuEntry("Agregar archivo",1);
    glutAddMenuEntry("Guardar grafo",2);
    glutAddMenuEntry("Prediccion de frase",3);
    glutAddMenuEntry("Frase mas comun dada una palabra",4);
    glutAddMenuEntry("Eliminar grafo",5);
    glutAddMenuEntry("Creditos",6);
    glutAddMenuEntry("Salir",7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
    //glutKeyboardFunc(teclado);
}

static void grafos(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1,1,1);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (0,840,600,0);
    
    cantidad_elementos(raiz);
	asignar_posiciones(raiz,x,y,1);
    crea_aristas(raiz);
    crea_nodo(raiz);
    rango=0;

	glColor3d(red(255),green(255),blue(255));	
	glBegin (GL_QUADS);
		glVertex2f(0,0);
		glVertex2f(140,0);
		glVertex2f(140,105);
		glVertex2f(0,105);
	glEnd();

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,14.0f);
	sprintf(frase,"Menu: clic derecho");
	texto2(frase);

    glColor3d(red(218),green(247),blue(166));	
	glBegin (GL_QUADS);
        glVertex2f(8,20);
		glVertex2f(20,20);
		glVertex2f(20,30);
		glVertex2f(8,30);
	glEnd();

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(27.0f,30.0f);
	sprintf(frase,"Origen");
	texto2(frase);

    glColor3d(red(255),green(195),blue(0));	
	glBegin (GL_QUADS);
        glVertex2f(8,36);
		glVertex2f(20,36);
		glVertex2f(20,46);
		glVertex2f(8,46);
	glEnd();

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(27.0f,46.0f);
	sprintf(frase,"Destino");
	texto2(frase);

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,62.0f);
	sprintf(frase,"Up/Down: Mov. Horizontal");
	texto2(frase);

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,78.0f);
	sprintf(frase,"Lef/Right: Mov. Vertical");
	texto2(frase);

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,94.0f);
	sprintf(frase,"F4: Salir");
	texto2(frase);

	glPopMatrix();
	glutSwapBuffers();
}

void texto(char *frase){
	for(int i=0;i<strlen(frase);i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,frase[i]);
}

void texto2(char *frase){
	for(int i=0;i<strlen(frase);i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,frase[i]);
}

static void redimensiona(int ancho, int alto){
    const float ar=(float)ancho/(float)alto;

    glViewport(0,0,ancho, alto);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void asignar_posiciones(struct nodo *grafo,int posx,int posy,int contador){
	if(!grafo) return;
	grafo->x=posx;
	grafo->y=posy;

	if(contador==rango){
		posx=x-tam;
	    posy=posy+tam;
		contador=0;
	}
	contador++;
	asignar_posiciones(grafo->siguiente,posx+tam,posy,contador);
}

void crea_aristas(struct nodo *grafo){
    if(!grafo) return;
    une_aristas(grafo, grafo->aristas);
    crea_aristas(grafo->siguiente);

}

void une_aristas(struct nodo *grafo, struct arista *arista){
    if(!grafo || !arista) return;

    glLineWidth(2);
	glColor3d(1,1,1);
	glBegin (GL_LINES);
		glVertex2f(grafo->x,grafo->y);
		glVertex2f(arista->vertice->x,arista->vertice->y);
	glEnd();

    //Destino
    glPointSize(7);
    glColor3d(red(255),green(195),blue(0));	
	glBegin(GL_POINTS);
        glVertex2f((grafo->x+(8/3)*arista->vertice->x)/(10/3),(grafo->y+(8/3)*arista->vertice->y)/(10/3));
	glEnd();

    //Origen
    glPointSize(7);
    glColor3d(red(218),green(247),blue(166));
	glBegin(GL_POINTS);
        glVertex2f(((8/3)*grafo->x+arista->vertice->x)/(10/3),((8/3)*grafo->y+arista->vertice->y)/(10/3));
	glEnd();
	
    //Peso
	glColor3d(red(255), green(102), blue(0));
	glRasterPos2f(((grafo->x+arista->vertice->x)/2)+2,((grafo->y+arista->vertice->y)/2)-2);
	sprintf(frase,"%.2f",arista->peso);
	texto2(frase);

    une_aristas(grafo, arista->siguiente);
}

void crea_nodo(struct nodo *grafo){
    if(!grafo) return;

    glBegin(GL_POLYGON);
	glColor3d(red(199),green(0),blue(57));
	float px=0,py=0;
	for (float i=0;i<10;i=i+0.01){
		px=53*cos(i)+grafo->x;
		py=53*sin(i)+grafo->y;
		glVertex2f(px,py);
	}
	glEnd();

    glBegin(GL_POLYGON);
	glColor3d(red(255),green(255),blue(255));
	px=0, py=0;
	for (float i=0;i<10;i=i+0.01){
		px=50*cos(i)+grafo->x;
		py=50*sin(i)+grafo->y;
		glVertex2f(px,py);
	}
	glEnd();
    
    glColor3d(0,0,0);
	glRasterPos2f(grafo->x-(tam/6),grafo->y);
	sprintf(frase,grafo->palabra);
	texto(frase);

	glColor3d(0,0,0);
	glRasterPos2f(grafo->x-(tam/6),grafo->y+(tam/12));
	sprintf(frase,"%.4f",grafo->ocurrencia);
	texto(frase);

	crea_nodo(grafo->siguiente);   
}

void teclas_direccion(int tecla, int posx, int posy){
	if(tecla==GLUT_KEY_UP) y=y+10;
	if(tecla==GLUT_KEY_DOWN) y=y-10;
	if(tecla==GLUT_KEY_LEFT) x=x+10;
	if(tecla==GLUT_KEY_RIGHT) x=x-10;
    if(tecla==GLUT_KEY_F4) exit(0);
}

void actualizar(){
    glutSetWindow(ventana);
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
	if(ventana2!=0){
		glutSetWindow(ventana2);
		glutPostRedisplay();
	}
}

double red(double color){
    return color/255;
}

double green(double color){
    return color/255;
}

double blue(double color){
    return color/255;
}

void menu_principal(int opcion){
    /*
    glutAddMenuEntry("Agregar archivo",1);
    glutAddMenuEntry("Guardar grafo",2);
    glutAddMenuEntry("Prediccion de frase",3);
    glutAddMenuEntry("Frase mas comun dada una palabra",4);
    glutAddMenuEntry("Eliminar grafo",5);
    */
   switch(opcion){
       case 1:{ //Agregar que pida el nombre del archivo desde la pantalla
            lee_archivo(&raiz, "prueba.txt");
            prob_ocurrencia(&raiz);
            teorema_bayes(raiz);
            ordena_nodos(raiz);
            imprime_lista(raiz);
            printf("\n\nHay %i elementos\n", elementos);
           break;
       };
       case 2:{ //Que imprima en pantalla que se guardo satisfactoriamente
           guarda_grafo(raiz, "backup.txt");
           break;
       };
       case 3:{ //Prediccion jeje
           break;
       };
       case 4:{ //Frase mas comun jeje :v
           break;
       };
       case 5:{
           elimina_nodos(&raiz);
           break;
       };
       case 6:{ //Imprime en pantalla creditos
            break;
       };
       case 7:{
           exit(0);
           break;
       };
   }
   

}
