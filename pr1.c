/*Realizar un árbol B+ que:
*- Elimine 
*- Fusione - 2 puntos
- Busque 
- Muestre el camino de busqueda.
- Imprima que operaciones realizo durante las restructuraciones. - 3 puntos
- Medir el tiempo de búsqueda. (time.h)

- que guarde y carga en un archivo binario los datos del árbol - 3 puntos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define d 2 //Este numero corresponde al orden 

// En esta estructura se almacenaran los datos del paciente
struct nodo{
    int id;
    char nombre[30];
    char segundo_nombre[30];
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

//Funciones que crean las estructuras necesarias para que el programa funcione
struct nodo *crea_nodo(int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac);
struct elemento_pagina *crea_elemento_pagina();
struct pagina *crea_pagina();

//Funciones necesarias para la insercion
int inserta_nodo(struct pagina **raiz, int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar);
int inserta_nodo_pagina(struct elemento_pagina **inicio, int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar);
int insertar_actual(struct elemento_pagina **inicio, struct elemento_pagina *dato);
int contador_elementos(struct elemento_pagina *inicio);
struct elemento_pagina **obtener_centro(struct elemento_pagina **raiz, int contador);

//Funciones que permiten mostrar el arbol B+ en su totalidad
void mostrar_arbol(struct elemento_pagina *elemento, int contador);
void arbol_hijos(struct pagina *listado, int contador, int hoja);

//Funciones de archivos
int guarda_arbol(struct elemento_pagina *arbol);
void escribe_datos(FILE *archivo, struct elemento_pagina *arbol);
void acceso_recursivo_arbol(FILE *archivo, struct pagina *listado);
//int agrega_arbol();

//Busqueda
int buscar_dato(struct elemento_pagina *arbol, int id_pac);
void auxiliar_busqueda(struct pagina *listado, int id_pac);

void creditos();

int main(int argc, char const *argv[])
{
    struct pagina *raiz=NULL;
    struct elemento_pagina *res= NULL;
    int opcion, id_pac, edad_pac, contador, op_arch;
    char nombre_pac[30], nombre2_pac[30], ap_pac[30], am_pac[30];
    float peso_pac;
    do{
        printf("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
        printf("                    P A C I E N T E S\n\n");
        printf("Bienvenid@! Por favor, selecciona una opcion de la siguiente lista\n\n");
        printf("\t[1]: Agregar paciente\n\t[2]: Eliminar paciente\n\t[3]: Buscar paciente\n\t[4]: Mostrar lista de pacientes\n\t[5]: Cargar archivo guardado de pacientes\n\t[6]: Guardar archivo actual de pacientes\n\t[7]: Mostrar creditos\n\t[8]: Salir\n\n\tOpcion: ");
        scanf("%i", &opcion);
        switch(opcion){
            case 1:{
                printf("R E G I S T R O   D E   P A C I E N T E S\n\n");
                /*printf("Ingresa los siguientes datos del paciente a registrar\n");
                printf("ID: ");
                scanf("%i", &id_pac);
                printf("Nombre: ");
                scanf("%s", nombre_pac);
                printf("Segundo nombre (de no tener, ingresa X): ");
                scanf("%s", nombre2_pac);
                printf("Apellido paterno: ");
                scanf("%s", ap_pac);
                printf("Apellido materno: ");
                scanf("%s", am_pac);
                printf("Edad: ");
                scanf("%i", &edad_pac);
                printf("Peso: ");
                scanf("%f", &peso_pac);
                inserta_nodo(&raiz, id_pac, nombre_pac, nombre2_pac, ap_pac, am_pac, edad_pac, peso_pac, &res);
                res=NULL;*/
                inserta_nodo(&raiz,6,"Andres","","Mendez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,5,"Susana","","Sanchez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,7,"Margarita","","Ruiz","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,3,"Pedro","","Loyo","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,1,"Juan","","Ocampo","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,4,"Lucia","","Perez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,12,"Esteban","","Hernandez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,11,"Maria","","Lopez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,8,"Edith","","Ortiz","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,9,"Angel","","Mercado","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,10,"Salma","","Romero","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,2,"Perla","","Portugal","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,14,"Marcos","","Cruz","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,13,"Jose","","Rodriguez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,15,"Adriana","","Alvarez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,16,"Adriana","","Alvarez","Palacios",20,33.55,&res);
                res = NULL;
                printf("Pacientes registrados correctamente!\n\n");
                break;
            };
            case 2:{
                printf("E L I M I N A C I O N   D E   P A C I E N T E \n\n");
                break;
            };
            case 3:{
                printf("B U S Q U E D A   D E   P A C I E N T E \n\n");
                if(raiz && raiz->inicio){
                    printf("Ingresa el ID del paciente a buscar\n");
                    printf("ID: ");
                    scanf("%i", &id_pac);
                    if(buscar_dato(raiz->inicio, id_pac)==0) printf("Paciente localizado\n\n");
                    else printf("No se encontro al paciente\n\n");
                }
                else printf("-No existen pacientes registrados-\n");
                break;
            };
            case 4:{
                printf("L I S T A D O   D E   P A C I E N T E S\n\n");
                if(raiz && raiz->inicio){
                    contador=0;
                    mostrar_arbol(raiz->inicio, contador);
                }
                else printf("-No existen pacientes registrados-\n");
                break;
            };
            case 5:{
                printf("C A R G A R   A R C H I V O   D E   P A C I E N T E S\n\n");
                break;
            };
            case 6:{
                printf("G U A R D A R   A R C H I V O   D E   P A C I E N T E S\n\n");
                printf("\nSelecciona una de las siguientes opciones\n\t[1]: Agregar datos a base de datos existente\n\t[2]: Sobreescribir base de datos existente\n\n\tOpcion: ");
                scanf("%i", &op_arch);
                switch (op_arch){
                    case 1:{
                        break;
                    };
                    case 2:{
                        if(raiz && raiz->inicio){
                            if(guarda_arbol(raiz->inicio)==0) printf("\nDatos guardados satisfactoriamente\n\n");
                            else printf("\nNo fue posible guardar la informacion\n\n");
                        }
                        else printf("-No existe ningun dato para almacenar-\n");
                        break;
                    };
                    default:{
                        break;
                    };
                }
                break;
            };
            case 7:{
                printf("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n\n\n\n");
                creditos();
                break;
            };
            case 8:{
                printf("\n\n*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
                printf("Hasta luego!! :)\n\n");
                break;
            };
            default:{
                printf("    \n\nADVERTENCIA: Opcion invalida\n\n");
                break;
            };
        }
    }
    while(opcion!=8);
    return 0;
}

struct elemento_pagina *crea_elemento_pagina(){
    return (struct elemento_pagina*)malloc(sizeof(struct elemento_pagina));
}

struct pagina *crea_pagina(){
    return (struct pagina*)malloc(sizeof(struct pagina));
}

struct nodo *crea_nodo(int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac){
    struct nodo *pNuevo=(struct nodo*)malloc(sizeof(struct nodo));
    if(!pNuevo) return NULL;

    pNuevo->id = id_pac;
    strcpy(pNuevo->nombre,nom_pac);
    strcpy(pNuevo->segundo_nombre,nom2_pac);
    strcpy(pNuevo->apellido_paterno,ap_pac);
    strcpy(pNuevo->apellido_materno,am_pac);
    pNuevo->edad=edad_pac;
    pNuevo->peso=peso_pac;
    pNuevo->izquierda= NULL;
    pNuevo->derecha= NULL;
    return pNuevo;
}

int inserta_nodo(struct pagina **raiz, int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar){
    if(!*raiz){ //Si no existe el elemento pagina "raiz", entonces...
        if(!(*raiz=crea_pagina())) return -1; // Se crea la pagina con la funcion y si no se pudo crear entonces termina la funcion
        (*raiz)->inicio=NULL; //Primero apuntamos el inicio a NULL para después aplicarle a este inicio la función nodo_pagina
        if(!(inserta_nodo_pagina(&((*raiz)->inicio), id_pac, nom_pac, nom2_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar))) return -1;

        return 0;
    }

    // A partir de aqui se va a definir si se insertara como hoja o dentro de la pagina
    inserta_nodo_pagina(&((*raiz)->inicio), id_pac, nom_pac, nom2_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar); // Comenzamos insertando el nodo en la pagina

    if(*pAuxiliar){
        insertar_actual(&(*raiz)->inicio, *pAuxiliar); //Esta funcion va a poner a pAuxiliar en la raiz
    }

    int cant_elementos=contador_elementos((*raiz)->inicio);
    
    if (cant_elementos>2*d){ //Esta funcion es la que nos va a permitir partir la pagina
        //Calculamos el centro en la pagina actual
        struct elemento_pagina **centro=obtener_centro(&(*raiz)->inicio,0); 
        //Si el centro no tiene hijos...
        if(!((*centro)->paciente->izquierda || (*centro)->paciente->derecha)){
            struct elemento_pagina *pCopia=NULL;
            
            pCopia=crea_elemento_pagina();
            pCopia->paciente=NULL;
            pCopia->siguiente=NULL;
            pCopia->paciente=crea_nodo((*centro)->paciente->id,"","", "", "", 0,0);

            if (!(centro || pCopia)) return 0;
        
            struct pagina *izquierda=crea_pagina();
            struct pagina *derecha=crea_pagina();
            if (!(izquierda || derecha)) return 0;

            pCopia->paciente->izquierda=izquierda;
            pCopia->paciente->derecha=derecha;

            izquierda->inicio=(*raiz)->inicio;
            derecha->inicio=(*centro);
            
            (*raiz)->inicio=pCopia;
            pCopia->siguiente=NULL;
            *pAuxiliar=(*raiz)->inicio;
    
            (*centro)=NULL;          
            return 0;
        }
        else{            //Pero si si tiene hijos, entonces hay que tratarlo basicamente como un arbol B pues la clave vacia debe de subir
            if (!(centro)) return 0;
        
            struct pagina *izquierda=crea_pagina();
            struct pagina *derecha=crea_pagina();
            if (!(izquierda || derecha)) return -1;

            (*centro)->paciente->izquierda=izquierda;
            (*centro)->paciente->derecha=derecha;

            izquierda->inicio=(*raiz)->inicio;
            derecha->inicio=(*centro)->siguiente;         
            
            (*raiz)->inicio=*centro;
            (*centro)->siguiente=NULL;
            *pAuxiliar=(*raiz)->inicio;

            (*centro)=NULL;  
            return 0;
        }
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
    if (!*inicio){
        *inicio=pEPagina;
        pEPagina->siguiente=NULL;
        return 0;
    }
    if(((*inicio)->paciente->id > pEPagina->paciente->id)){
        if((*inicio)->paciente->izquierda && pEPagina->paciente->izquierda ){
            pEPagina->paciente->derecha=(*inicio)->paciente->izquierda;
        }
        pEPagina->siguiente=*inicio;
        *inicio=pEPagina;
        return 0;
    }
    if(!(*inicio)->siguiente){
        if((*inicio)->paciente->izquierda && pEPagina->paciente->izquierda ){
            (*inicio)->paciente->derecha=pEPagina->paciente->izquierda;
        }
        (*inicio)->siguiente=pEPagina;
        pEPagina->siguiente=NULL;
        return 0;
    }
    return insertar_actual(&(*inicio)->siguiente, pEPagina);
}

//La siguiente funcion se encargara de insertar el nodo en el elemento pagina,
// servira mas que nada para ubicar si tiene que ir a la derecha o a la izquierda
int inserta_nodo_pagina(struct elemento_pagina **inicio, int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac, struct elemento_pagina **pAuxiliar){
    if(!*inicio){
        if(!(*inicio=crea_elemento_pagina())) return -1; // Se trata de crear un elemento pagina en inicio, si no se pudo retorna a 0
        (*inicio)->paciente=NULL;
        if(!((*inicio)->paciente=crea_nodo(id_pac,nom_pac, nom2_pac, ap_pac,am_pac,edad_pac,peso_pac))) return -1; // se trata de crear el nodo del paciente en inicio->paciente, si no se pudo retorna a 0
        
        (*inicio)->siguiente=NULL;
        return 0;
    }

    if((*inicio)->paciente->id>id_pac){ // Se comienza a insertar del lado izquierdo si el id de la raiz es mayor al que se va a ingresar
        if((*inicio)->paciente->izquierda) return inserta_nodo(&((*inicio)->paciente->izquierda), id_pac, nom_pac, nom2_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar);// Si hay hijos a la izquierda, entonces es posible insertarlo abajo a la izquierda
        
        //Sino, entonces lo insertamos en la pagina actual como si se tratase de una lista :)
        struct elemento_pagina *pNuevo=NULL; // Lo declaramos como NULL ya que en una función más adelante se declarará el alojamiento de memoria
        
        if(!(pNuevo=crea_elemento_pagina())) return -1;
        pNuevo->paciente=NULL;
        pNuevo->siguiente=NULL;
        if(!(pNuevo->paciente=crea_nodo(id_pac, nom_pac, nom2_pac, ap_pac, am_pac, edad_pac, peso_pac))) return -1;
        
        pNuevo->siguiente=*inicio;
        *inicio=pNuevo;

        return 0;
    }
    if(!(*inicio)->siguiente){
        if((*inicio)->paciente->derecha) return inserta_nodo(&((*inicio)->paciente->derecha), id_pac, nom_pac, nom2_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar);
    }
    return inserta_nodo_pagina(&((*inicio)->siguiente),id_pac, nom_pac, nom2_pac, ap_pac, am_pac, edad_pac, peso_pac, pAuxiliar);
}

void creditos(){
    printf("Este proyecto fue realizado por la estudiante\nZulema Concepción Luis Cruz (S18001442)\nde la carrera Ingenieria Informatica\nde la Universidad Veracruzana\n\n");
    printf("\nEs un programa que realiza una base de datos de los pacientes\ndentro de un hospital haciendo uso de recursividad, archivos\ny Arboles B+\n");
    printf("                                          _\n");
    printf("Programa realizado para la materia de Diseno de Programas\nimpartida por el Dr. Luis Felipe Marin Urias\n\n");
    printf("\n\nDiciembre 2020\n");
    return;
}

void mostrar_arbol(struct elemento_pagina *elemento, int contador){
    if(elemento){
        for(int i=0; i<contador; i++) printf("   ");
        if(strcmp(elemento->paciente->nombre, "")==0) printf("|__ Clave: %i\n", elemento->paciente->id);
        else printf("|--%i. %s %s\n", elemento->paciente->id, elemento->paciente->nombre, elemento->paciente->apellido_paterno);

        arbol_hijos(elemento->paciente->izquierda, contador+1, 1);
        arbol_hijos(elemento->paciente->derecha, contador+1, 2);
        mostrar_arbol(elemento->siguiente, contador);
    }
    return;
}

void arbol_hijos(struct pagina *listado, int contador, int hoja){
    if(listado){
        for(int i=0; i<contador; i++) printf("   ");
        printf("|-*HOJA %i*-\n", hoja);
        mostrar_arbol(listado->inicio, contador);
        for(int i=0; i<contador; i++) printf("   ");
        printf("|\n");
    }
    return;
}

int guarda_arbol(struct elemento_pagina *arbol){
    FILE *archivo;
    
    archivo=fopen("bd_hospital.zule", "ab");
    remove("bd_hospital.zule"); //Borramos archivo para evitar duplicidad de datos
    archivo=fopen("bd_hospital.zule", "ab"); //Y lo volvemos a abrir

    if(!archivo){
        printf("No se pudo crear el archivo\n\n");
        fclose(archivo);
        return -1;
    }

    escribe_datos(archivo, arbol);

    fclose(archivo);
    return 0;
}

void escribe_datos(FILE *archivo, struct elemento_pagina *arbol){
    if(arbol){        
        int estado; //Comprueba que el nodo tenga un hijo a la derecha y que lo comparta con el nodo siguiente:)
        if(arbol->paciente->derecha) estado=1;
        if(arbol->siguiente){
            if(arbol->siguiente->paciente->izquierda) estado++;
        }

        if(estado==2){  //En esta seccion unicamente se comprueba que no se comparta el puntero para no copiarlo dos veces
            struct pagina *pActual=arbol->paciente->derecha;
            struct pagina *pSiguiente=arbol->siguiente->paciente->izquierda;
            if(pActual==pSiguiente){
                if(strcmp(arbol->paciente->nombre, "")==0); //Asi no guardaremos en el fichero las claves que contienen datos vacios
                else{
                    fwrite(&(arbol->paciente), sizeof(struct nodo), 1, archivo);
                    //printf("|- %i %s %s\n", arbol->paciente->id, arbol->paciente->nombre, arbol->paciente->apellido_paterno);
                }
                acceso_recursivo_arbol(archivo, arbol->paciente->izquierda);
                escribe_datos(archivo, arbol->siguiente);
            }
        }
        else{
            if(strcmp(arbol->paciente->nombre, "")==0); //Asi no guardaremos en el fichero las claves que contienen datos vacios
            else{
                fwrite(&(arbol->paciente), sizeof(struct nodo), 1, archivo);
                //printf("|- %i %s %s\n", arbol->paciente->id, arbol->paciente->nombre, arbol->paciente->apellido_paterno);
            }
            acceso_recursivo_arbol(archivo, arbol->paciente->izquierda);
            escribe_datos(archivo, arbol->siguiente);
            acceso_recursivo_arbol(archivo, arbol->paciente->derecha);
        }
    }
    return;
}
void acceso_recursivo_arbol(FILE *archivo, struct pagina *listado){
    if(listado){
        escribe_datos(archivo, listado->inicio);
    }
    return;
}

int buscar_dato(struct elemento_pagina *arbol, int id_pac){
    if(arbol){
        if(id_pac>arbol->paciente->id){ //Si id_pac es mayor que el nodo que esta presente se va a la derecha
            printf("Comparando...\n%i > %i\n", id_pac, arbol->paciente->id);
            auxiliar_busqueda(arbol->paciente->derecha, id_pac);
            buscar_dato(arbol->siguiente, id_pac);
            auxiliar_busqueda(arbol->paciente->izquierda, id_pac);
        }
        if(id_pac<arbol->paciente->id){ //Si id_pac es menor se va a la izquierda
            printf("Comparando...\n%i < %i\n", id_pac, arbol->paciente->id);
            auxiliar_busqueda(arbol->paciente->izquierda, id_pac);
            buscar_dato(arbol->siguiente, id_pac);
            auxiliar_busqueda(arbol->paciente->derecha, id_pac);
        }
        if(arbol->paciente->id==id_pac) return 0;
        //buscar_dato(arbol->siguiente, id_pac);
    }
    else return -1;
}

void auxiliar_busqueda(struct pagina *listado, int id_pac){
    if(listado){
        buscar_dato(listado->inicio, id_pac);
    }
    return;
}
