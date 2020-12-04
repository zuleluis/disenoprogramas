/*Realizar un árbol B+ que:

- Inserte 
*- Elimine 
*- Fusione - 2 puntos
- Busque 
- Muestre el camino de busqueda.
- Imprima que operaciones realizo durante las restructuraciones. - 3 puntos
OK - El dato de los nodos debe contener información de pacientes de un hospital. 
- Medir el tiempo de búsqueda. (time.h)
- Comprobar que funciona y que lo hace correctamente en depuración (debug) - 2 puntos
- Imprimir el árbol en consola de tipo: - 3 puntos
Raíz
|__rama                                         
    |__subrama
    |   |___hoja 1
    |   |   |- dato paciente 1
    |   |   |- dato paciente n
    |   |___hoja 2
    |   |   |- dato paciente 1
    |   |   |- dato paciente n  
    |__subrama
        ...

- que guarde y carga en un archivo binario los datos del árbol - 3 puntos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define d 2

// En esta estructura se almacenaran los datos del paciente
struct nodo{
    int id;
    char nombres[30];
    char apellido_paterno[30];
    char apellido_materno[30];
    int edad;
    float peso; //Variable que almacena el peso del paciente en kg
    struct pagina *izquierda;
    struct pagina *derecha; 
};

struct elemento_pagina{ // Se almacena cada nodo dentro del elemento pagina
    struct nodo *paciente;
    struct elemento_pagina *siguiente;
};

struct pagina{ // Pagina completa
    struct elemento_pagina *inicio;
};

struct nodo *crea_nodo(int id_pac, char *nom_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac);
struct elemento_pagina *crea_elemento_pagina();
struct pagina *crea_pagina();

int contador_elementos(struct elemento_pagina *inicio);
struct elemento_pagina **obtener_centro(struct elemento_pagina **raiz, int contador);
int insertar_actual(struct elemento_pagina **inicio, struct elemento_pagina *dato);

int inserta_nodo(struct pagina **raiz, int id_pac, char *nom_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar);
int inserta_nodo_pagina(struct elemento_pagina **inicio, int id_pac, char *nom_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar);
int redirecciona_derecha(struct pagina **arbol);

void mostrar_arbol(struct elemento_pagina *listado);

int main(int argc, char const *argv[])
{
    struct pagina *raiz=NULL;
    struct elemento_pagina *res= NULL;
    
    printf("Inicia Programa\n");
    inserta_nodo(&raiz,6,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,5,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,7,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,3,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,1,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,4,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,12,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,11,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,8,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,9,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,10,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    inserta_nodo(&raiz,2,"Andres","Mercado","Palacios",20,33.55,&res);
    res = NULL;
    redirecciona_derecha(&raiz);
    mostrar_arbol(raiz->inicio);

    return 0;
}

struct elemento_pagina *crea_elemento_pagina(){
    return (struct elemento_pagina*)malloc(sizeof(struct elemento_pagina));
}

struct pagina *crea_pagina(){
    return (struct pagina*)malloc(sizeof(struct pagina));
}

struct nodo *crea_nodo(int id_pac, char *nom_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac){
    struct nodo *pNuevo=(struct nodo*)malloc(sizeof(struct nodo));
    if(!pNuevo) return NULL;

    pNuevo->id = id_pac;
    strcpy(pNuevo->nombres,nom_pac);
    strcpy(pNuevo->apellido_paterno,ap_pac);
    strcpy(pNuevo->apellido_materno,am_pac);
    pNuevo->edad=edad_pac;
    pNuevo->peso=peso_pac;
    pNuevo->izquierda= NULL;
    pNuevo->derecha= NULL;
    return pNuevo;
}

int inserta_nodo(struct pagina **raiz, int id_pac, char *nom_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar){
    if(!*raiz){ //Si no existe el elemento pagina "raiz", entonces...
        if(!(*raiz=crea_pagina())) return -1; // Se crea la pagina con la funcion y si no se pudo crear entonces termina la funcion
        (*raiz)->inicio=NULL; //Primero apuntamos el inicio a NULL para después aplicarle a este inicio la función nodo_pagina
        if(!(inserta_nodo_pagina(&((*raiz)->inicio), id_pac, nom_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar))) return -1;

        return 0;
    }

    // A partir de aqui se va a definir si se insertara como hoja o dentro de la pagina
    inserta_nodo_pagina(&((*raiz)->inicio), id_pac, nom_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar); // Comenzamos insertando el nodo en la pagina

    if(*pAuxiliar){
        insertar_actual(&(*raiz)->inicio, *pAuxiliar); //Esta funcion va a poner a pAuxiliar en la raiz
    }

    int cant_elementos=contador_elementos((*raiz)->inicio);
    
    if (cant_elementos>2*d){ //Esta funcion es la que nos va a permitir partir la pagina
        struct elemento_pagina **centro=obtener_centro(&(*raiz)->inicio,0); 
        struct elemento_pagina *pCopia=NULL;
        
        pCopia=crea_elemento_pagina();
        pCopia->paciente=NULL;
        pCopia->siguiente=NULL;
        pCopia->paciente=crea_nodo((*centro)->paciente->id, "", "", "", 0,0);


        if (!(centro || pCopia)) return 0;
        /*
            3    5
        1 2  3 4  5 6 7 8
        */


        struct pagina *izquierda=crea_pagina();
        struct pagina *derecha=crea_pagina();
        if (!(izquierda || derecha)) return 0;

        izquierda->inicio=(*raiz)->inicio;
        derecha->inicio=(*centro);

        pCopia->paciente->izquierda=izquierda;
        pCopia->paciente->derecha=derecha;
        
        (*raiz)->inicio=pCopia;
        *pAuxiliar=(*raiz)->inicio;

        (*centro)=NULL;          
        return 0;
    }
    return 0;
}

int contador_elementos(struct elemento_pagina *inicio){
    if(inicio) return 1+contador_elementos(inicio->siguiente);
    else return 0;
}

struct elemento_pagina **obtener_centro(struct elemento_pagina **raiz, int contador){
    if(!*raiz) return NULL;
    if(contador==d) return raiz;
    return obtener_centro(&((*raiz)->siguiente), contador+1);
}

int insertar_actual(struct elemento_pagina **inicio, struct elemento_pagina *pEPagina){
    if (!*inicio)
    {
        *inicio=pEPagina;
        pEPagina->siguiente=NULL;
        return 0;
    }
    if(((*inicio)->paciente->id > pEPagina->paciente->id))
    {
        pEPagina->siguiente=*inicio;
        *inicio=pEPagina;
        return 0;
    }
    return insertar_actual(&(*inicio)->siguiente, pEPagina);
    
    /*
    SE CANCELA PQ ME DEJA AFUERA UNOS DATOS
    HAY QUE HACER LO K DECIA DE REDIRECCIONAR
    CHICLE Y PEGA
    if(!(*inicio)->siguiente){ 
        pEPagina->paciente->izquierda=(*inicio)->paciente->derecha;
        (*inicio)->siguiente=pEPagina;
        pEPagina->siguiente=NULL;
        return 0;
    }
    if((*inicio)->paciente->id> pEPagina->paciente->id){
        pEPagina->siguiente=*inicio;
        *inicio=pEPagina;
        (*inicio)->siguiente->paciente->izquierda=(*inicio)->paciente->derecha;
        return 0;
    }
    return insertar_actual(&(*inicio)->siguiente, pEPagina);*/
}

//La siguiente funcion se encargara de insertar el nodo en el elemento pagina,
// servira mas que nada para ubicar si tiene que ir a la derech o a la izquierda
int inserta_nodo_pagina(struct elemento_pagina **inicio, int id_pac, char *nom_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar){
    if(!*inicio){
        if(!(*inicio=crea_elemento_pagina())) return -1; // Se trata de crear un elemento pagina en inicio, si no se pudo retorna a 0
        (*inicio)->paciente=NULL;
        if(!((*inicio)->paciente=crea_nodo(id_pac,nom_pac,ap_pac,am_pac,edad_pac,peso_pac))) return -1; // se trata de crear el nodo del paciente en inicio->paciente, si no se pudo retorna a 0
        
        (*inicio)->siguiente=NULL;
        return 0;
    }

    if((*inicio)->paciente->id>id_pac){ // Se comienza a insertar del lado izquierdo si el id de la raiz es mayor al que se va a ingresar
        if((*inicio)->paciente->izquierda) return inserta_nodo(&((*inicio)->paciente->izquierda), id_pac, nom_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar);// Si hay hijos a la izquierda, entonces es posible insertarlo abajo a la izquierda
        
        //Sino, entonces lo insertamos en la pagina actual como si se tratase de una lista :)
        struct elemento_pagina *pNuevo=NULL; // Lo declaramos como NULL ya que en una función más adelante se declarará el alojamiento de memoria
        
        if(!(pNuevo=crea_elemento_pagina())) return -1;
        pNuevo->paciente=NULL;
        pNuevo->siguiente=NULL;
        if(!(pNuevo->paciente=crea_nodo(id_pac, nom_pac, ap_pac, am_pac, edad_pac, peso_pac))) return -1;
        
        pNuevo->siguiente=*inicio;
        *inicio=pNuevo;

        return 0;
    }
    if(!(*inicio)->siguiente){
        if((*inicio)->paciente->derecha) return inserta_nodo(&((*inicio)->paciente->derecha), id_pac, nom_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar);
    }
    return inserta_nodo_pagina(&((*inicio)->siguiente),id_pac, nom_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar);
}

int redirecciona_derecha(struct pagina **arbol){
    if((*arbol)){
        struct elemento_pagina *pAux=(*arbol)->inicio;
        while(pAux){
            if(pAux->paciente->izquierda && pAux->paciente->derecha && pAux->siguiente)
                pAux->paciente->derecha=pAux->siguiente->paciente->izquierda;
            pAux=pAux->siguiente;
        }
        return 0;
    }
    else return -1;
}

void mostrar_arbol(struct elemento_pagina *listado){
    if(listado){
        struct elemento_pagina *pAux=listado;
        while(pAux){
            //printf("PAGINA RAIZ\n\tId: %i\n\tNombre: %s\n\tApellidos: %s %s\n\tEdad: %i\n\tPeso: %.2f kg\n\n ", pAux->paciente->id, pAux->paciente->nombres, pAux->paciente->apellido_paterno, pAux->paciente->apellido_materno, pAux->paciente->edad, pAux->paciente->peso);
            printf("%i\n", pAux->paciente->id);

            if(pAux->paciente->izquierda && pAux->paciente->derecha){
                struct elemento_pagina *pAuxi=pAux->paciente->izquierda->inicio;
                struct elemento_pagina *pAuxd=pAux->paciente->derecha->inicio;
                while(pAuxi){
                // printf("Izquierda\n\tId: %i\n\tNombre: %s\n\tApellidos: %s %s\n\tEdad: %i\n\tPeso: %.2f kg\n\n ", pAuxi->paciente->id, pAuxi->paciente->nombres, pAuxi->paciente->apellido_paterno, pAuxi->paciente->apellido_materno, pAuxi->paciente->edad, pAuxi->paciente->peso);
                    printf("Izquierda: %i\n", pAuxi->paciente->id);
                    pAuxi=pAuxi->siguiente;
                }

                while(pAuxd){
                    //printf("Derecha\n\tId: %i\n\tNombre: %s\n\tApellidos: %s %s\n\tEdad: %i\n\tPeso: %.2f kg\n\n ", pAuxd->paciente->id, pAuxd->paciente->nombres, pAuxd->paciente->apellido_paterno, pAuxd->paciente->apellido_materno, pAuxd->paciente->edad, pAuxd->paciente->peso);
                    printf("Derecha: %i\n", pAuxd->paciente->id);
                    pAuxd=pAuxd->siguiente;
                }
            }
            pAux=pAux->siguiente;
        }
    }
    return;
}
