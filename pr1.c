/*Realizar un árbol B+ que:
*- Elimine 
*- Fusione - 2 puntos
- Imprima que operaciones realizo durante las restructuraciones. - 3 puntos

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

struct datos{ //Una estructura auxiliar para la carga de archivos
    int id;
    char nombre[30];
    char segundo_nombre[30];
    char apellido_paterno[30];
    char apellido_materno[30];
    int edad;
    float peso; //Variable que almacena el peso del paciente en kg
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
int lee_archivo(struct pagina **raiz, struct elemento_pagina **pAuxiliar);

//Busqueda
int buscar_dato(struct elemento_pagina *arbol, int id_pac);
int busqueda_invisible(struct elemento_pagina *arbol, int id_pac); //Esta funcion es para comprobar que el ID a insertar no esta registrado previamente

void creditos();

int main(int argc, char const *argv[])
{
    struct pagina *raiz=NULL;
    raiz->inicio==NULL;
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
                if(raiz){
                    if(busqueda_invisible(raiz->inicio, id_pac)==0){
                        printf("ERROR: El ID a ingresar ya existe en el sistema");
                        break;
                    }
                }
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
                /*inserta_nodo(&raiz,6,"Andres","","Mendez","Palacios",20,33.55,&res);
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
                inserta_nodo(&raiz,16,"Lucia","","Perez","Palacios",20,33.55,&res);
                res = NULL;*/

                inserta_nodo(&raiz,1,"Andres","","Mendez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,2,"Susana","","Sanchez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,3,"Margarita","","Ruiz","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,4,"Pedro","","Loyo","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,5,"Juan","","Ocampo","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,6,"Lucia","","Perez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,7,"Esteban","","Hernandez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,8,"Maria","","Lopez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,9,"Edith","","Ortiz","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,10,"Angel","","Mercado","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,11,"Salma","","Romero","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,12,"Perla","","Portugal","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,13,"Marcos","","Cruz","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,14,"Jose","","Rodriguez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,15,"Adriana","","Alvarez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,16,"Lucia","","Perez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,17,"Domingo","","Ramirez","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,18,"Paola","","Reyes","Palacios",20,33.55,&res);
                res = NULL;
                inserta_nodo(&raiz,19,"Esteban","","Cruz","Palacios",20,33.55,&res);
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
                    //clock_t es un tipo de dato que generalmente es long int
                    clock_t inicio, fin;
                    double tiempo;

                    inicio=clock();
                    if(buscar_dato(raiz->inicio, id_pac)!=0) printf("El paciente no fue localizado\n\n");
                    fin=clock();
                    //Se realiza un cast de clock_t a double para poder realizar la operacion
                    //y se divide entre 
                    tiempo=((double)(fin-inicio)/CLOCKS_PER_SEC);
                    //Se multiplica para convertir segundos a milisegundos
                    printf("Tiempo de ejecucion: %.4f milisegundos\n\n", tiempo*1000);
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
                printf("Estas seguro?\n\t[1]: Si\n\t[Cualquier otro numero]: No\n\n\tOpcion: ");
                scanf("%i", &op_arch);
                switch(op_arch){
                    case 1:{
                        if(lee_archivo(&raiz, &res)==0) printf("Archivo cargado con exito\n\n");
                        else printf("ERROR: No fue posible cargar la base de datos requerida\n\n");
                        break;
                    };
                    default:{
                        break;
                    };
                }
                break;
            };
            case 6:{
                printf("G U A R D A R   A R C H I V O   D E   P A C I E N T E S\n\n");
                printf("\nSelecciona una de las siguientes opciones\n\t[1]: Agregar datos a base de datos existente\n\t[2]: Sobreescribir base de datos existente\n\t[Otro numero]: Regresar al menu\n\n\tOpcion: ");
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
                        printf("Regresando al menu...\n\n");
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
    printf("Creando elemento pagina...\n");
    return (struct elemento_pagina*)malloc(sizeof(struct elemento_pagina));
}

struct pagina *crea_pagina(){
    printf("Creando pagina...\n");
    return (struct pagina*)malloc(sizeof(struct pagina));
}

struct nodo *crea_nodo(int id_pac, char *nom_pac, char *nom2_pac, char *ap_pac, char *am_pac, int edad_pac, float peso_pac){
    struct nodo *pNuevo=(struct nodo*)malloc(sizeof(struct nodo));
    if(!pNuevo) return NULL;

    printf("Creando nuevo nodo...\n");
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
    printf("\n\nInsertando nodo %i..\n", id_pac);
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
        *pAuxiliar=NULL;
    }

    int cant_elementos=contador_elementos((*raiz)->inicio);
    
    if (cant_elementos>2*d){ //Esta funcion es la que nos va a permitir partir la pagina
        //Calculamos el centro en la pagina actual
        printf("Realizando division...\n");
        struct elemento_pagina **centro=obtener_centro(&(*raiz)->inicio,0); 
        //Si el centro no tiene hijos...
        if(!(*centro)->paciente->izquierda || !(*centro)->paciente->derecha){
            printf("El centro no tiene hijos...\n");
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
            printf("Asignando apuntador izquierdo a la copia clave...\n");
            pCopia->paciente->derecha=derecha;
            printf("Asignando apuntador derecho a la copia clave...\n");

            izquierda->inicio=(*raiz)->inicio;
            printf("Izquierda comienza en %i...\n", izquierda->inicio->paciente->id);
            derecha->inicio=(*centro);
            printf("Derecha comienza en %i...\n", derecha->inicio->paciente->id);
            
            (*raiz)->inicio=pCopia;
            pCopia->siguiente=NULL;
            *pAuxiliar=(*raiz)->inicio;
    
            (*centro)=NULL;          
            return 0;
        }
        else{//Pero si si tiene hijos, entonces hay que tratarlo basicamente como un arbol B pues la clave vacia debe de subir
            if (!(centro)) return 0;
            printf("El centro tiene hijos\n");
        
            struct pagina *izquierda=crea_pagina();
            struct pagina *derecha=crea_pagina();
            if (!(izquierda || derecha)) return -1;

            (*centro)->paciente->izquierda=izquierda;
            printf("Asignando apuntador izquierdo...\n");
            (*centro)->paciente->derecha=derecha;
            printf("Asignando apuntador derecho...\n");

            izquierda->inicio=(*raiz)->inicio;
            printf("Izquierda comienza en %i...\n", izquierda->inicio->paciente->id);
            derecha->inicio=(*centro)->siguiente;
            printf("Derecha comienza en %i...\n", derecha->inicio->paciente->id);         
            
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
    if(contador==d){
        printf("El centro es %i \n", (*raiz)->paciente->id);
        return raiz;
    }
    return obtener_centro(&((*raiz)->siguiente), contador+1);
}

int insertar_actual(struct elemento_pagina **inicio, struct elemento_pagina *pEPagina){
    printf("Insertamos pAuxiliar, nodo actual %i...\n", (*inicio)->paciente->id);
    if (!*inicio){
        printf("No hay inicio...\n");
        *inicio=pEPagina;
        pEPagina->siguiente=NULL;
        return 0;
    }
    if(((*inicio)->paciente->id > pEPagina->paciente->id)){
        printf("%i en raiz es mayor que %i a insertar\n",(*inicio)->paciente->id, pEPagina->paciente->id);
        if((*inicio)->paciente->izquierda && pEPagina->paciente->izquierda ){
            pEPagina->paciente->derecha=(*inicio)->paciente->izquierda;
        }
        pEPagina->siguiente=*inicio;
        *inicio=pEPagina;
        return 0;
    }
    if(!(*inicio)->siguiente){
        printf("%i es el ultimo elemento de la pagina actual, %i lo insertamos a la derecha\n", (*inicio)->paciente->id, pEPagina->paciente->id);
        if((*inicio)->paciente->derecha && pEPagina->paciente->izquierda ){
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
    struct datos pNuevo;
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
                    pNuevo.id=arbol->paciente->id;
                    strcpy(pNuevo.nombre, arbol->paciente->nombre);
                    strcpy(pNuevo.segundo_nombre, arbol->paciente->segundo_nombre);
                    strcpy(pNuevo.apellido_paterno, arbol->paciente->apellido_paterno);
                    strcpy(pNuevo.apellido_materno, arbol->paciente->apellido_materno);
                    pNuevo.edad=arbol->paciente->edad;
                    pNuevo.peso=arbol->paciente->peso;
                    fwrite(&pNuevo, sizeof(struct datos), 1, archivo);
                    //printf("|- %i %s %s\n", arbol->paciente->id, arbol->paciente->nombre, arbol->paciente->apellido_paterno);
                }
                acceso_recursivo_arbol(archivo, arbol->paciente->izquierda);
                escribe_datos(archivo, arbol->siguiente);
            }
        }
        else{
            if(strcmp(arbol->paciente->nombre, "")==0); //Asi no guardaremos en el fichero las claves que contienen datos vacios
            else{
                pNuevo.id=arbol->paciente->id;
                strcpy(pNuevo.nombre, arbol->paciente->nombre);
                strcpy(pNuevo.segundo_nombre, arbol->paciente->segundo_nombre);
                strcpy(pNuevo.apellido_paterno, arbol->paciente->apellido_paterno);
                strcpy(pNuevo.apellido_materno, arbol->paciente->apellido_materno);
                pNuevo.edad=arbol->paciente->edad;
                pNuevo.peso=arbol->paciente->peso;
                fwrite(&pNuevo, sizeof(struct datos), 1, archivo);
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
        //Primero, se hara la comparacion de si el ID a buscar coincide con el ID encontrado siempre y cuando no sea un nodo auxiliar (o nulo, en este caso)
        //Esto nos asegura que ya estamos en la fila de las hojas 
        if(id_pac==arbol->paciente->id && strcmp(arbol->paciente->nombre,"")!=0){
            //printf("Comparando...\n%i = %i\n\n", id_pac, arbol->paciente->id);
            printf("-> %i = %i\n\n", id_pac, arbol->paciente->id);
            printf("P A C I E N T E   L O C A L I Z A D O\n\n");
            printf("ID: %i\nNombre(s): %s %s\n", arbol->paciente->id, arbol->paciente->nombre, arbol->paciente->segundo_nombre);
            printf("Apellido paterno: %s\nApellido materno: %s\nEdad: %i\nPeso: %.2f kg\n\n", arbol->paciente->apellido_paterno, arbol->paciente->apellido_materno, arbol->paciente->edad, arbol->paciente->peso);
            return 0;
        }
        else{
            if(id_pac>=arbol->paciente->id){ //Si id_pac es mayor que el nodo que esta presente se va a la derecha
                //printf("Comparando...\n%i - %i\n", id_pac, arbol->paciente->id);
                printf("-> %i ", arbol->paciente->id);
                //SI el ID a buscar es mayor que el que se esta comparando pero existe la posibilidad de seguir avanzando en la fila
                //Para compararlo con los otros nodos, entonces avanzamos
                if(id_pac>arbol->paciente->id && arbol->siguiente) return buscar_dato(arbol->siguiente, id_pac);
                else{ //Si no, entonces bajamos a la derecha siempre y cuando exista, sino, significa que ya estamos en las hojas y por ende la busqueda termina
                    if(arbol->paciente->derecha){
                        //printf("\nEs momento de bajar a la derecha\n\n");
                        printf("\n");
                        return buscar_dato(arbol->paciente->derecha->inicio, id_pac);
                    }
                    else{
                        printf("\nNo es posible seguir buscando\n");
                        return -1;
                    }
                }
            }
            else{ //Si id_pac es menor se va para abajo, a la izquierda
                //printf("Comparando...\n%i - %i\n", id_pac, arbol->paciente->id);
                printf("-> %i ", arbol->paciente->id);
                if(arbol->paciente->izquierda){ //Si es posible seguir bajando lo haremos, sino, entonces ya estamos en las hojas y la busqueda termina
                    //printf("\nEs momento de bajar a la izquierda\n\n");
                    printf("\n");
                    return buscar_dato(arbol->paciente->izquierda->inicio, id_pac);
                }
                else{
                    printf("\nNo es posible seguir buscando\n");
                    return -1;   
                }
            }
        }
    }
    else return -1;

}

int busqueda_invisible(struct elemento_pagina *arbol, int id_pac){
    if(arbol){
        if(id_pac==arbol->paciente->id && strcmp(arbol->paciente->nombre,"")!=0) return 0;
        else{
            if(id_pac>=arbol->paciente->id){ 
                if(id_pac>arbol->paciente->id && arbol->siguiente) return busqueda_invisible(arbol->siguiente, id_pac);
                else{
                    if(arbol->paciente->derecha) return busqueda_invisible(arbol->paciente->derecha->inicio, id_pac);
                    else return -1;
                }
            }
            else{
                if(arbol->paciente->izquierda) return busqueda_invisible(arbol->paciente->izquierda->inicio, id_pac);
                else return -1;   
            }
        }
    }
    else return -1;
}


int lee_archivo(struct pagina **raiz, struct elemento_pagina **pAuxiliar){
    FILE *archivo;
    
    archivo=fopen("bd_hospital.zule", "rb");

    if(!archivo) return -1;

    struct datos pNuevo;

    while(!feof(archivo)){
        while(fread(&pNuevo, sizeof(struct datos), 1, archivo)){
			inserta_nodo(&*raiz, pNuevo.id, pNuevo.nombre, pNuevo.segundo_nombre, pNuevo.apellido_paterno, pNuevo.apellido_materno, pNuevo.edad, pNuevo.peso, pAuxiliar);
            *pAuxiliar=NULL;
		}
    }
    fclose(archivo);
    return 0;
}
