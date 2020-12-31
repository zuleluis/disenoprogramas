/*
Una empresa de aplicaciones móviles quiere desarrollar su propio software de texto predictivo basado en distribución probabilística,
por lo que debe realizar un programa que utilice un grafo de palabras conectadas por la frecuencia basado en un diccionario (texto
de carga de palabras). 

Las palabras se van conectando entre sí por la secuencia del texto y será interrumpida sólo por signos de puntuación (punto, comas,
punto y coma, signos de admiración, etc). 

Cada nodo almacena la palabra y su probabilidad de ocurrencia P(A) donde se determina como:
P(A)= d/m
Donde A es el nodo, d es el número de ocurrencias de la palabra contenida en A y m el número total de palabras que forman el texto.

El peso de una arista que conecte dos nodos estará determinado por la fórmula bayesiana de eventos dependientes:

           P(B|A)*P(A)
P(A|B)=-------------------
              P(B)
 
Donde b denota la palabra antecesora para p(B) diferente de 0, y
         
         P(BnA)
P(B|A)= --------- si P(A) es diferente de 0
          P(A)

donde P(AnB) es la probabilidad conjunta de que ambas palabras se encuentren en la misma oración (para evitar una recursión infinita) y
P(BnA)=P(AnB)
 
Hacer un programa en C que de modo gráfico usando OpenGL:
    - Genere y muestre un grafo conectado y dirigido, basado en las palabras del archivo.
    - Guarde el grafo y que pueda anexar otro archivo de texto al grafo, actualizando el grafo existente y sin borrar los datos previos.
    - Muestre nodos y sus valores (P(A))
    - Muestre aristas y sus pesos (P(A|B))
    - Permita capturar en tiempo de ejecución una frase y que palabra por palabra haga la predicción de la palabra siguiente basándose en el camino más
      corto (Dijkstra) desde la primera palabra capturada, una vez capturada debe actualizar el grafo. (2 PTS)
    - Tenga una opción para desplegar las frases más comunes ordenadas de la más común a la menos, dada una palabra inicial por el usuario. (2 PTS)
    - Debe mostrar las palabras y las frases en un color distinto.
    - Tener una opción de limpiar todo el grafo y volver a empezar con uno nuevo.
*/

//#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo{
    char palabra[30];
    int x;
    int y;
    float ocurrencia; //Guarda la probabilidad de ocurrencia P(A)=d/m
    int frecuencia; //Guarda la frecuencia con la que aparece cada palabra
    int frecuencia_aristas; 
    struct nodo *siguiente;
    struct arista *aristas;
};

struct arista{
    int peso;
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


int lee_archivo(struct nodo **raiz, char *nombre_archivo);
char evalua_caracter_especial(char letra);
int guarda_grafo(struct nodo *raiz, char *nombre_archivo);

int prob_ocurrencia(struct nodo **grafo);

int elementos=0; //Cantidad de elementos en el grafo

int main(int argc, char const *argv[])
{
    struct nodo *raiz=NULL;
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

    inserta_arista(raiz, "hola", "esto");
    inserta_arista(raiz, "esto", "es");
    inserta_arista(raiz, "es", "una");
    inserta_arista(raiz, "una", "prueba");
    inserta_arista(raiz, "prueba", "para");
    inserta_arista(raiz, "para", "ver");
    inserta_arista(raiz, "ver", "si");
    inserta_arista(raiz, "si", "esto");
    inserta_arista(raiz, "esto", "funciona");*/

    lee_archivo(&raiz, "xd");
    prob_ocurrencia(&raiz);
    imprime_lista(raiz);
    printf("\n\nHay %i elementos\n", elementos);
    //guarda_grafo(raiz, "ok");

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
    if(grafo){
        printf("%s: %f, %i veces\n", grafo->palabra, grafo->ocurrencia, grafo->frecuencia);
        imprime_lista(grafo->siguiente);
    }
}

/*int imprime_aristas(struct arista *arista){
    if(!arista) return 0;
    im
}*/

/*
void p_bayes(vertice *raiz){
	if(!raiz)
		return;
	asignar_pesos_bayes(raiz,raiz->aristas);
	p_bayes(raiz->siguiente);
}

void asignar_pesos_bayes(vertice *origen,arista *destino){
	if(!destino)
		return;
	float b_inter_a=destino->frecuencia/origen->frecuencia_aristas;
	destino->costo=b_inter_a/origen->ocurrencia;
	asignar_pesos_bayes(origen,destino->siguiente);
}
*/

int lee_archivo(struct nodo **grafo, char *nombre_archivo){
    FILE *archivo;

    //archivo=fopen(nombre_archivo, "rb+");
    archivo=fopen("prueba.txt", "rb+");

    if(!archivo) return -1;
    char letra, auxiliar;

    char palabra[30], origen[30], destino[30];
    int i=0, inicio=0;
    printf("\n");
    while(!feof(archivo)){ //Leemos letra a letra el archivo
        fread(&letra,sizeof(char),1,archivo);
        if(letra==-61){
            fread(&letra,sizeof(char),1,archivo);
            letra=evalua_caracter_especial(letra);
        }
        if(letra>=97 && letra<=122 || letra>=65 && letra<=90){
            palabra[i]=letra;
            i++;
        }
        else{
            if(!feof(archivo)){
                i=0;
                printf("Palabra: %s\n", palabra);
                inserta_nodo(grafo, palabra);
                elementos++;
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
                memset(palabra, '\0', 20);
            }
        }
    }
    fclose(archivo);
    return 0;
}

char evalua_caracter_especial(char letra){
    if(letra==-95 || letra==-127) return 'a';
    if(letra==-87 || letra==-119) return 'e';
    if(letra==-83 || letra==-115) return 'i';
    if(letra==-77 || letra==-109) return 'o';
    if(letra==-70 || letra==-102) return 'u';
    if(letra==-79 || letra==-111) return 'n';
}

int guarda_grafo(struct nodo *grafo, char *nombre_archivo){
    FILE *archivo;

    archivo=fopen("archivo2.txt", "w+");
    if(!archivo) return -1;
    
    while(grafo){
        fwrite(grafo,sizeof(struct nodo),1,archivo);
        grafo=grafo->siguiente;
    }

    fclose(archivo);
    return 0;

}

int prob_ocurrencia(struct nodo **grafo){
    if(!*grafo) return 0;
    (*grafo)->ocurrencia=(float)(*grafo)->frecuencia/elementos;
    prob_ocurrencia(&(*grafo)->siguiente);
}
