/*
Hacer un programa en C que de modo gráfico usando OpenGL:
    - Lea un archivo dado por el usuario, que contenga un texto normal (Ver archivo anexo).
    - Genere y muestre un grafo conectado y dirigido, basado en las palabras del archivo.
    - Guarde el grafo y que pueda anexar otro archivo de texto al grafo, actualizando el
      grafo existente y sin borrar los datos previos.
    - Muestre nodos y sus valores (P(A))
    - Muestre aristas y sus pesos (P(A|B))
    - Permita capturar en tiempo de ejecución una frase y que palabra por palabra haga la
      predicción de la palabra siguiente basándose en el camino más corto (Dijkstra) desde
      la primera palabra capturada, una vez capturada debe actualizar el grafo. (2 PTS)
    - Tenga una opción para desplegar las frases más comunes ordenadas de la más común a
         la menos, dada una palabra inicial por el usuario. (2 PTS)
    - Debe mostrar las palabras y las frases en un color distinto.
    - Tener una opción de limpiar todo el grafo y volver a empezar con uno nuevo.
*/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

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
int frases_comunes(struct nodo *grafo, int limite);
int frases_comunes_aristas(struct arista *arista,int limite);
void concatena_frase(char *palabra, int contador);
int elimina_nodos(struct nodo **grafo);
void elimina_aristas(struct arista **arista);
int cantidad_elementos(struct nodo *grafo);

int lee_archivo(struct nodo **grafo, char *nombre_archivo);
char evalua_caracter_especial(char letra);
void borra_archivo_backup();

int prob_ocurrencia(struct nodo **grafo);
int teorema_bayes(struct nodo *grafo);
int bayes_aristas(struct nodo *anterior, struct arista **arista);

//Funciones de graficos
void inicializa();
static void grafos(void);
void texto(char *frase);
void texto2(char *frase);
void texto3(char *frase);
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
static void teclado_agregar(unsigned char tecla, int x, int y);
static void agregar_archivo(void);
static void teclado_buscar(unsigned char tecla, int x, int y);
static void buscar_frases_comunes(void);
static void teclado_predicciones(unsigned char tecla, int x, int y);
static void prediccion_palabras(void);
int agrega_frase_archivo();
//Ventanas auxiliares
static void guardado_exitosamente(void);
static void error(void);
static void cargando(void);
static void grafo_eliminado(void);
static void creditos(void);


//Variables globales
int elementos=0; //Cantidad de elementos en el grafo
int x=100, y=100, rango=0, tam=200; // X: Posicion en X, Y: Posicion en Y, Rango: Ayuda a distribuir los nodos a traves de la ventana. Es decir, si hay 10 elementos, en cada linea habran 3 elementos 
struct nodo *raiz=NULL;             //Tam: Distancia entre un nodo y otro. A mayor tamaño mayor distancia
char frase[50], frase_2[50], frase_3[150], nombrearchivo[30], palabra_actual[30], palabra_siguiente[30], frase_agregar[150], frase_comun[150], palabra_buscada[30], palabra_1[30], palabra_2[30], palabra_3[30], palabra_4[30], palabra_5[30];
int ventana=0,ventana2=0; //Ventana: Ventana principal, ventana2: Ventana secundaria
int temporizador, posicion=0, posicion_frase=0, estado=3, estado_prediccion=0, estado_oracion=1;
int posy=5, posy_aux=0; //Para desplazamiento en segunda ventana
bool mitad_frase; //Evalua si en la prediccion la palabra a buscar se encontraba a mitad de frase o no


int main(int argc, char *argv[]){
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
    char nombre_backup[20];
    char letra, auxiliar;
    if(strcmp(nombre_archivo, "backup.txt")==0){
        strcpy(nombre_backup, "backup(2).txt");
    }
    else strcpy(nombre_backup, "backup.txt");

    FILE *archivo, *backup;
    archivo=fopen(nombre_archivo, "r+");
    backup=fopen(nombre_backup, "a+"); //Como actualizacion para seguir agregando datos

    if(!archivo) return -1;
    if(!backup) return -1;

    //Copiamos el archivo en el backup
    while(!feof(archivo)){
        fread(&letra,sizeof(char),1,archivo);
        fwrite(&letra,sizeof(char),1,backup);
    }
    fclose(archivo);
    fclose(backup);

    archivo=fopen(nombre_archivo, "r+");    

    char palabra[30], origen[30], destino[30];
    int i=0, inicio=0;

    while(!feof(archivo)){ //Leemos letra a letra el archivo
        fread(&letra,sizeof(char),1,archivo);

        if(letra==-61){
            fread(&letra,sizeof(char),1,archivo);
            letra=evalua_caracter_especial(letra);
        }

        if(letra>=65 && letra<=90) letra=tolower(letra);

        if(letra>=97 && letra<=122){
            palabra[i]=letra;
            i++;
        }

        if(letra=='.' || letra==',' || letra=='-' || letra=='?' || letra=='!' || letra==32 || letra=='\n' || letra=='\r' || letra==';' || letra==26 || letra==-62){
            if(letra==-62 || letra==-65) fread(&letra,sizeof(char),1,archivo);
            palabra[i]='\0';
            if(palabra[0]!=0){
                elementos++;
                //printf("Palabra: %s\n", palabra);
                inserta_nodo(grafo, palabra);
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
            i=0;
            memset(palabra, '\0', 30);
        }
    }
    printf("Hay %i elementos registrados\n\n", elementos);
    fclose(archivo);
    return 0;
}

//Convierte caracteres especiales a caracteres comunes
char evalua_caracter_especial(char letra){
    if(letra==-95 || letra==-127) return 'a';
    if(letra==-87 || letra==-119) return 'e';
    if(letra==-83 || letra==-115) return 'i';
    if(letra==-77 || letra==-109) return 'o';
    if(letra==-70 || letra==-102 || letra==-68) return 'u';
    if(letra==-79 || letra==-111) return 'n';
    else return 'x';
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

int frases_comunes(struct nodo *grafo, int limite){
	if(!grafo) return -1;

	if((!grafo->aristas && limite>0) || limite==5){ //El limite maximo de palabras por frase, en esta ocasion, es seis (la palabra guardada + cinco mas)
        concatena_frase(grafo->palabra, limite); //Se asigna la ultima palabra

        //Se une todo en un arreglo en comun a excepcion de la palabra 1 ya que esta es la que se busco
        //printf("%s -> ", palabra_buscada);

        strcat(frase_comun, palabra_1);
        strcat(frase_comun, " ");
        strcat(frase_comun, palabra_2);
        strcat(frase_comun, " ");
        strcat(frase_comun, palabra_3);
        strcat(frase_comun, " ");
        strcat(frase_comun, palabra_4);
        strcat(frase_comun, " ");
        strcat(frase_comun, palabra_5);
        strcat(frase_comun, " ");

        //printf("%s", frase_comun);
        //printf("\n\n");

        glColor3d(red(0),green(0), blue(100));
        glRasterPos2f(-96.0f,posy_aux);
        strcpy(frase_2,palabra_buscada);
        texto(frase_2);

        glColor3d(red(0),green(0), blue(0));
        glRasterPos2f(-80.0f,posy_aux);
        strcpy(frase_3,frase_comun);
        texto(frase_3);

        posy_aux=posy_aux-15;

        memset(frase_comun, '\0', strlen(frase_comun)); //Vaciando frase comun
        return 0;
	}
    
    concatena_frase(grafo->palabra, limite);
	frases_comunes_aristas(grafo->aristas,limite+1);
}

int frases_comunes_aristas(struct arista *arista,int limite){
	if(!arista) return 0;
    frases_comunes(arista->vertice,limite);
    frases_comunes_aristas(arista->siguiente,limite);
}

void concatena_frase(char *palabra, int contador){ //Funcion que ayuda a evitar perder las palabras repetidas de frases anteriores
	/*//Con esta propuesta de ciclo se pierde la frase que pueda llegar a repetirse
        strcat(frase_comun, palabra);
        strcat(frase_comun," ");
	*/
    if(contador==0) strcpy(palabra_buscada,palabra);
    if(contador==1) strcpy(palabra_1,palabra);
    if(contador==2) strcpy(palabra_2,palabra);
    if(contador==3)	strcpy(palabra_3,palabra);
    if(contador==4)	strcpy(palabra_4,palabra);
    if(contador==5) strcpy(palabra_5,palabra);
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
    glutAddMenuEntry("Capturar una frase",2);
    glutAddMenuEntry("Frase mas comun dada una palabra",3);
    glutAddMenuEntry("Eliminar grafo",4);
    glutAddMenuEntry("Creditos",5);
    glutAddMenuEntry("Salir",6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
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
	strcpy(frase,"Menu: clic derecho");
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
	strcpy(frase,"Origen");
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
	strcpy(frase,"Destino");
	texto2(frase);

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,62.0f);
	strcpy(frase,"Up/Down: Mov. Horizontal");
	texto2(frase);

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,78.0f);
	strcpy(frase,"Lef/Right: Mov. Vertical");
	texto2(frase);

    glColor3d(0.30,0.30,0.30);
	glRasterPos2f(7.0f,94.0f);
	strcpy(frase,"F4: Salir");
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

void texto3(char *frase){
    for(int i=0;i<strlen(frase);i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,frase[i]);
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
		posx=x-tam; //X regresa al inicio de la fila
	    posy=posy+tam; //Y baja una posicion hacia abajo
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
	strcpy(frase,grafo->palabra);
	texto(frase);

	glColor3d(0,0,0);
	glRasterPos2f(grafo->x-(tam/6),grafo->y+(tam/12));
	sprintf(frase,"%.4f",grafo->ocurrencia);
	texto(frase);

	crea_nodo(grafo->siguiente);   
}

void teclas_direccion(int tecla, int posx, int posy){
	if(tecla==GLUT_KEY_UP) y=y+30;
	if(tecla==GLUT_KEY_DOWN) y=y-30;
	if(tecla==GLUT_KEY_LEFT) x=x+30;
	if(tecla==GLUT_KEY_RIGHT) x=x-30;
    if(tecla==GLUT_KEY_F4) exit(0);
}

void actualizar(){
    glutSetWindow(ventana);
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();

	if(ventana2!=0){ 
		glutSetWindow(ventana2);
		glutPostRedisplay();
        if(temporizador==1){
            sleep(1.5);
            glutDestroyWindow(ventana2);
            ventana2=0;
            temporizador=0;
        }
        if(temporizador==2){
            sleep(4);
            glutDestroyWindow(ventana2);
            ventana2=0;
            temporizador=0;
        }
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
    if(ventana2!=0){ //En dado caso de que por X o Y no se haya destruido la ventana 2, la eliminamos aqui
        glutDestroyWindow(ventana2);
        ventana2=0;
    }
    switch(opcion){
        case 1:{
            ventana2=glutCreateSubWindow(ventana,270,225,300,150);
            glutKeyboardFunc(teclado_agregar);
            glutDisplayFunc(agregar_archivo);
            break;
        };
        case 2:{
            ventana2=glutCreateSubWindow(ventana,270,225,300,150);
            glutKeyboardFunc(teclado_predicciones);
            glutDisplayFunc(prediccion_palabras);
            break;
        };
        case 3:{
            ventana2=glutCreateSubWindow(ventana,0,500,840,100);
            glutKeyboardFunc(teclado_buscar);
            glutDisplayFunc(buscar_frases_comunes);
            break;
        };
        case 4:{
            ventana2=glutCreateSubWindow(ventana,270,225,300,150);
            elimina_nodos(&raiz);
            if(!raiz){
                glutDisplayFunc(grafo_eliminado); 
                x=100; 
                y=100;
                rango=0;
                tam=200;
            }
            else glutDisplayFunc(error);
            temporizador=1;
            break;
        };
        case 5:{
            ventana2=glutCreateSubWindow(ventana,0,0,840,600);
            glutDisplayFunc(creditos);
            temporizador=2;
            break;
        };
        case 6:{
            borra_archivo_backup();
            exit(0);
            break;
        };
   }
}

static void guardado_exitosamente(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-0.96,-0.91);
		glVertex2f(-0.96,0.91);
		glVertex2f(0.96,0.91);
		glVertex2f(0.96,-0.91);
	glEnd();

    glColor3d(0,0,0);
	glRasterPos2f(-0.65f, 0.1f);
    strcpy(frase, "Guardado exitosamente");
	texto3(frase);
    glRasterPos2f(-0.25f, -0.1f);
    strcpy(frase, "backup.txt");
    texto2(frase);
    glPopMatrix();
	glutSwapBuffers();
}

static void error(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();


    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-0.96,-0.91);
		glVertex2f(-0.96,0.91);
		glVertex2f(0.96,0.91);
		glVertex2f(0.96,-0.91);
	glEnd();

    glColor3d(0,0,0);
	glRasterPos2f(-0.25f, 0.0f);
    strcpy(frase, "ERROR!!");
	texto3(frase);
    glPopMatrix();
	glutSwapBuffers();
}

static void cargando(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();


    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-0.96,-0.91);
		glVertex2f(-0.96,0.91);
		glVertex2f(0.96,0.91);
		glVertex2f(0.96,-0.91);
	glEnd();

    glColor3d(0,0,0);
	glRasterPos2f(-0.70f, 0.0f);
    strcpy(frase, "Cargando archivo...");
	texto3(frase);
    glPopMatrix();
	glutSwapBuffers();
}

static void grafo_eliminado(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();


    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-0.96,-0.91);
		glVertex2f(-0.96,0.91);
		glVertex2f(0.96,0.91);
		glVertex2f(0.96,-0.91);
	glEnd();

    glColor3d(0,0,0);
	glRasterPos2f(-0.45f, 0.0f);
    strcpy(frase, "Grafo eliminado");
	texto3(frase);
    glPopMatrix();
	glutSwapBuffers();
}

static void creditos(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

    glColor3d(red(255),green(255),blue(255));	
	glBegin (GL_QUADS);
        glVertex2f(-0.7,0.85);
		glVertex2f(0.7,0.85);
		glVertex2f(0.7,-0.7);
		glVertex2f(-0.7,-0.7);
	glEnd();

    glColor3d(red(240),green(133),blue(240));	
	glBegin (GL_QUADS);
        glVertex2f(-0.7,-0.7);
		glVertex2f(-0.7,0.7);
		glVertex2f(0.7,0.7);
		glVertex2f(0.7,-0.7);
	glEnd();

    glColor3d(0,0,0);
	glRasterPos2f(-0.25f, 0.75f);
    strcpy(frase, "DISENO DE PROGRAMAS");
	texto3(frase);

    glColor3d(1,1,1);
	glRasterPos2f(-0.25f, 0.5f);
    strcpy(frase, "Universidad Veracruzana");
	texto3(frase);

    glColor3d(1,1,1);
	glRasterPos2f(-0.20f, 0.35f);
    strcpy(frase, "Programa Educativo");
	texto3(frase);

    glColor3d(1,1,1);
	glRasterPos2f(-0.21f, 0.25f);
    strcpy(frase, "Ingenieria Informatica");
	texto3(frase);

    glColor3d(1,1,1);
	glRasterPos2f(-0.23f, 0.05f);
    strcpy(frase, "Programa realizado por");
	texto3(frase);

    glColor3d(1,1,1);
	glRasterPos2f(-0.3f, -0.05f);
    strcpy(frase, "Zulema Concepcion Luis Cruz");
	texto3(frase);

    glColor3d(1,1,1);
	glRasterPos2f(-0.13f, -0.15f);
    strcpy(frase, "Enero 2021");
	texto3(frase);

    glPopMatrix();
	glutSwapBuffers();
}

static void teclado_agregar(unsigned char tecla, int x, int y){
    if((tecla>='a' && tecla<='z') || (tecla>='A' && tecla<='Z') || tecla=='_' || tecla=='-' || tecla=='.'|| (tecla>='0' && tecla<='9')){
        nombrearchivo[posicion]=tecla;
        posicion++;
    }

    if(tecla==8 && posicion>0){ //Tecla de retroceso
		posicion--;
        nombrearchivo[posicion]='\0';
	}

    if(tecla==13 && posicion>0){ //Tecla de enter
        strcat(nombrearchivo, ".txt");
        if(lee_archivo(&raiz, nombrearchivo)==0){
            prob_ocurrencia(&raiz);
            teorema_bayes(raiz);
            ordena_nodos(raiz);
            imprime_lista(raiz);
            memset(frase,'\0', 50);
            memset(nombrearchivo, '\0', 30);
            estado=1;
        }
        else{
            memset(frase,'\0', 50);
            memset(nombrearchivo, '\0', 30);
            estado=0;
        }

        glutDestroyWindow(glutGetWindow());
		ventana2=0;
        posicion=0;

        ventana2=glutCreateSubWindow(ventana,270,225,300,150);
        if(estado==1) glutDisplayFunc(cargando); 
        if(estado==0)glutDisplayFunc(error);
        estado=2;
        temporizador=1;
    }

    if(tecla==27){ //Tecla Esc
        glutDestroyWindow(glutGetWindow());
        memset(frase,'\0', 50);
        memset(nombrearchivo, '\0', 30);
		ventana2=0;
        posicion=0;
    }
}

static void agregar_archivo(void){
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-100,100,-50,50);

    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-96,-46);
		glVertex2f(-96,46);
		glVertex2f(96,46);
		glVertex2f(96,-46);
	glEnd();
	
	glColor3d(0,0,0);
	glRasterPos2f(-80.0f,36.0f);
	strcpy(frase,"Ingrese el nombre del archivo sin extension");
	texto(frase);

	glColor3d(0,0,0);
	glRasterPos2f(-93.0f,0.0f);
	strcpy(frase,"Archivo:");
	texto(frase);

	glColor3d(red(88),green(24), blue(69));
	glRasterPos2f(-60.0f,0.0f);
	strcpy(frase,nombrearchivo);
	texto(frase);

    glColor3d(0,0,0);
	glRasterPos2f(45.0f,-40.0f);
	strcpy(frase,"ESC: Cancelar");
	texto2(frase);

	glPopMatrix();
	glutSwapBuffers();
}

static void teclado_buscar(unsigned char tecla, int x, int y){
    if((tecla>='a' && tecla<='z') || (tecla>='A' && tecla<='Z') || tecla=='_' || tecla=='-' || tecla=='.'|| (tecla>='0' && tecla<='9')){
        if(estado==2 && posicion<30){ //Evitamos que siga registrando mas y mas datos despues de haber dado enter.
            palabra_buscada[posicion]=tecla;
            posicion++;
        }
    }

    if(tecla==8 && posicion>0){ //Tecla de retroceso
		posicion--;
        palabra_buscada[posicion]='\0';
	}

    if(tecla==13 && posicion>0){ //Tecla de enter
        struct nodo *pBusqueda=busca_nodo(raiz, palabra_buscada);
        if(pBusqueda){
            estado=1;
        }
        else{
            memset(frase,'\0', 50);
            memset(palabra_buscada, '\0', 30);
            estado=0;
            glutDisplayFunc(error);
            estado=2;
            temporizador=1;
            posicion=0;
        }
    }

    if((tecla=='w' || tecla=='W') && estado==1) posy=posy+10;
    if((tecla=='s' || tecla=='S') && estado==1) posy=posy-10;

    if(tecla==27){ //Tecla Esc
        glutDestroyWindow(glutGetWindow());
        memset(frase,'\0', 50);
        memset(palabra_buscada, '\0', 30);
		ventana2=0;
        posicion=0;
        estado=2;
    }
}

static void buscar_frases_comunes(void){
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-100,100,-50,50);

    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-99,-46);
		glVertex2f(-99,46);
		glVertex2f(99,46);
		glVertex2f(99,-46);
	glEnd();
	
	glColor3d(0,0,0);
	glRasterPos2f(-96.0f,30.0f);
	strcpy(frase,"Ingrese la palabra que desea buscar");
	texto(frase);

	glColor3d(0,0,0);
	glRasterPos2f(-96.0f,16.0f);
	strcpy(frase,"Palabra:");
	texto(frase);

	glColor3d(red(88),green(24), blue(69));
	glRasterPos2f(-85.0f,16.0f);
	strcpy(frase,palabra_buscada);
	texto(frase);

    glColor3d(0,0,0);
	glRasterPos2f(73.0f,-40.0f);
	strcpy(frase,"ESC: Cancelar/Salir");
	texto2(frase);

    if(estado==1){
        glColor3d(0,0,0);
        glRasterPos2f(-96.0f,-40.0f);
        strcpy(frase,"W/S: Desplazamiento");
        texto2(frase);
        posy_aux=posy;
        frases_comunes(busca_nodo(raiz, palabra_buscada),0);
    }

	glPopMatrix();
	glutSwapBuffers();
}

static void teclado_predicciones(unsigned char tecla, int x, int y){
    if((tecla>='a' && tecla<='z') || (tecla>='A' && tecla<='Z') || tecla==',' || tecla=='.' || tecla==';'|| (tecla>='0' && tecla<='9') || tecla=='!' || tecla=='?' || tecla=='"' || tecla==' ' && posicion_frase<149){
        if(tecla==' '){ //Separacion de palabra para buscarla y predecir la que sigue
            palabra_actual[posicion]='\0';

            //Aqui es donde se busca la palabra
            if(raiz){
                struct nodo *pBusqueda=busca_nodo(raiz, palabra_actual);
                if(pBusqueda){
                    if(pBusqueda->aristas->vertice) strcpy(palabra_siguiente, pBusqueda->aristas->vertice->palabra);
                }
                else memset(palabra_siguiente, '\0', strlen(palabra_siguiente));
            }

            memset(palabra_actual, '\0', strlen(palabra_actual));
                
            posicion=0;
            mitad_frase=false;
        }

        if(mitad_frase==false && tecla!=' '){
            palabra_actual[posicion]=tecla;
            posicion++;
        }
    
        frase_agregar[posicion_frase]=tecla;
        posicion_frase++;
    }

    if(tecla==8 && posicion_frase>0 && posicion>=0){ //Tecla de retroceso
		posicion--;
        posicion_frase--;
        frase_agregar[posicion_frase]='\0';
        memset(palabra_siguiente, '\0', strlen(palabra_siguiente));

        if(posicion>0) palabra_actual[posicion]='\0';
        else{
            if(frase_agregar[posicion_frase-1]!=' ') mitad_frase=true;
            else mitad_frase=false;
            posicion=0;
            memset(palabra_actual, '\0', 30);
        }
	}

    if(tecla==13 && posicion_frase>0){ //Tecla de enter
        agrega_frase_archivo();
        printf("Frase agregada: %s\n\n", frase_agregar);
        glutDestroyWindow(glutGetWindow());
        memset(frase,'\0', 50);
        memset(frase_agregar,'\0', 150);
        memset(palabra_actual, '\0', 30);
        memset(palabra_siguiente, '\0', 30);
		ventana2=0;
        posicion=0;
        posicion_frase=0;
        estado=2;
    }

    if(tecla==27){ //Tecla Esc
        glutDestroyWindow(glutGetWindow());
        memset(frase,'\0', 50);
        memset(frase_agregar,'\0', 150);
        memset(palabra_actual, '\0', 30);
        memset(palabra_siguiente, '\0', 30);
		ventana2=0;
        posicion=0;
        posicion_frase=0;
        estado=2;
    }
}

static void prediccion_palabras(void){
    glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (-100,100,-50,50);

    glColor3d(red(233),green(233),blue(233));	
	glBegin (GL_QUADS);
        glVertex2f(-96,-46);
		glVertex2f(-96,46);
		glVertex2f(96,46);
		glVertex2f(96,-46);
	glEnd();
	
	glColor3d(0,0,0);
	glRasterPos2f(-80.0f,36.0f);
	strcpy(frase,"Ingrese una frase");
	texto(frase);

	glColor3d(0,0,0);
	glRasterPos2f(-93.0f,0.0f);
	strcpy(frase,"Frase:");
	texto(frase);

	glColor3d(red(88),green(24), blue(69));
	glRasterPos2f(-60.0f,0.0f);
	strcpy(frase,frase_agregar);
	texto(frase);

    /*glColor3d(red(88),green(24), blue(69));
	glRasterPos2f(-60.0f,30.0f);
	strcpy(frase,palabra_actual);
	texto(frase);*/
    
    glColor3d(red(0),green(0), blue(200));
    glRasterPos2f(-60.0f,10.0f);
    strcpy(frase,palabra_siguiente);
    texto(frase);

    glColor3d(0,0,0);
	glRasterPos2f(45.0f,-40.0f);
	strcpy(frase,"ESC: Cancelar");
	texto2(frase);
	
	glPopMatrix();
	glutSwapBuffers();
}

int agrega_frase_archivo(){
    FILE *archivo;
    archivo=fopen("backup.txt", "a+");

    if(!archivo) return -1;

    int longitud=strlen(frase_agregar);
    char separar=',';
    char finalizar='\n';

    fwrite(&separar,sizeof(char),1,archivo);
    for(int i=0; i<longitud; i++){
        fwrite(&frase_agregar[i],sizeof(char),1,archivo);
    }
    fwrite(&finalizar,sizeof(char),1,archivo);

    fclose(archivo);
	elimina_nodos(&raiz);
	elementos=0;

	if (lee_archivo(&raiz,"backup.txt")==0){
        prob_ocurrencia(&raiz);
        teorema_bayes(raiz);
        ordena_nodos(raiz);
        imprime_lista(raiz);
        memset(frase,'\0', 50);
    }
}

void borra_archivo_backup(){
    rename("backup.txt", "backup_old.txt");
    return;
}
