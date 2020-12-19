#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *palabra;

struct estado{
	palabra *cadena;
	unsigned int q, r, p, b, c, r1, b1, c1; 
    //Por qué unsigned? Porque necesitamos trabajar unicamente con numeros positivos, no podemos
    //regresar a valores del vector con posiciones negativas
};

int compara(palabra a, palabra b);
void sube(unsigned int *a, unsigned int *b);
void baja(unsigned int *a, unsigned int *b);
void sift(struct estado *elemento);
void trinkle(struct estado *elemento);
void semitrinkle(struct estado *elemento);
void smoothsort(palabra *a, unsigned int longitud);

int main(int argc, const char **argv) {
	char *lista[] = {
        "perro", "gato", "oso"/*, "leon", "tigre", "lemur", "caballo"/*,
		"ardilla", "elefante", "suricata", "jirafa", "cebra", "lechuza",
		"delfin", "tiburon", "hiena", "zorro", "buho", "foca", "iguana", 
		"koala", "mariposa", "nutria", "quokka", "rinoceronte", "urraca",
		"venado", "xoloitzcuintle", "yegua"*/
	};
    int longitud=sizeof(lista)/sizeof(lista[0]);
	unsigned int i;

	smoothsort(lista, longitud);
	printf("\n-----------------------------\n");

    printf("La lista contiene %i elementos\n", longitud);
    printf("---------------------------------\n");
	for (i = 0; i < longitud; i++) {
		printf("[%i]: %s\n", i+1, lista[i]);
	}
	return 0;
}

int compara(palabra a, palabra b){
	return strcmp(a, b);
}

void smoothsort(palabra *palabra, unsigned int longitud){
	struct estado elemento;
	unsigned int temporal;

	elemento.cadena = palabra;
	printf("\n----SMOOTHSORT-----\n");
    printf("elemento.cadena -> %s\n", *palabra);
	elemento.r = 0;
	elemento.p = elemento.b = elemento.c = 1;

	for (elemento.q = 1; elemento.q < longitud; elemento.q++) {
		elemento.r1 = elemento.r;
		if ((elemento.p & 7) == 3){
			elemento.b1 = elemento.b;
			elemento.c1 = elemento.c;
			sift(&elemento);

			elemento.p = (elemento.p + 1) >> 2;

			temporal = elemento.b + elemento.c + 1;
			elemento.b = elemento.b + temporal + 1;
			elemento.c = temporal;
		} 
        else if ((elemento.p & 3) == 1){
			if (elemento.q + elemento.c < longitud){
				elemento.b1 = elemento.b;
				elemento.c1 = elemento.c;

				sift(&elemento);
			}
            else trinkle(&elemento);

			do{
				baja(&elemento.b, &elemento.c);
				elemento.p <<= 1;
			}
            while (elemento.b > 1);

			elemento.p++;
		}
		elemento.r++;
	}
	elemento.r1 = elemento.r;
	trinkle(&elemento);

	while (elemento.q-- > 1){
		if (elemento.b == 1){
			elemento.r--;
			elemento.p--;

			while((elemento.p & 1) == 0){
				elemento.p >>= 1;
				sube(&elemento.b, &elemento.c);
			}
		} 
        else if (elemento.b > 2){
			elemento.p--;
			elemento.r = elemento.r - elemento.b + elemento.c;

			if (elemento.p > 0) semitrinkle(&elemento);

			baja(&elemento.b, &elemento.c);

			elemento.p = (elemento.p << 1) + 1;
			elemento.r = elemento.r + elemento.c;
			semitrinkle(&elemento);

			baja(&elemento.b, &elemento.c);

			elemento.p = (elemento.p << 1) + 1;
		}
	}
}

void sube(unsigned int *a, unsigned int *b){
	printf("Subiendo...\n");
	unsigned int temporal;

	temporal = *a; 
	*a = *a + *b + 1;
	*b = temporal;
}

void baja(unsigned int *a, unsigned int *b){
	printf("Bajando...\n");
	unsigned int temporal;

	temporal = *b;
	*b = *a - *b - 1;
	*a = temporal;
}

void sift(struct estado *elemento){
	unsigned int r0, r2;
	palabra temporal;
	printf("\n------------SIFT--------------------\n");

	r0 = elemento->r1;
	temporal = elemento->cadena[r0];
	printf("temporal=elemento->cadena[r0] -> %s\n", temporal);

	while (elemento->b1 > 2) {
		r2 = elemento->r1 - elemento->b1 + elemento->c1;

		if (compara(elemento->cadena[elemento->r1 - 1], elemento->cadena[r2]) >= 0) {
			printf("elemento->cadena[elemento->r1 -1] -> %s\n", elemento->cadena[elemento->r1-1]);
			printf("elemento->cadena[elemento->r2] -> %s\n", elemento->cadena[r2]);
			r2 = elemento->r1 - 1;

			baja(&elemento->b1, &elemento->c1);
		}

		if (compara(elemento->cadena[r2], temporal) < 0) {
			printf("elemento->cadena[r2] -> %s\n", elemento->cadena[r2]);
			printf("temporal -> %s\n", temporal);
			elemento->b1 = 1;
		}
        else {
			elemento->cadena[elemento->r1] = elemento->cadena[r2];
			printf("elemento->cadena[elemento->r1] -> %s\n", elemento->cadena[elemento->r1]);
			elemento->r1 = r2;

			baja(&elemento->b1, &elemento->c1);
		}
	}

	if (elemento->r1 - r0 > 0) {
		elemento->cadena[elemento->r1] = temporal;
		printf("elemento->cadena[elemento->r1] -> %s\n", elemento->cadena[elemento->r1]);
	}
}

void trinkle(struct estado *elemento){
	unsigned int p1, r0, r2, r3;
	palabra temporal;

	p1 = elemento->p;
	elemento->b1 = elemento->b;
	elemento->c1 = elemento->c;
	r0 = elemento->r1; 
	temporal = elemento->cadena[r0];
	printf("\n------------TRINKLE--------------------\n");
	printf("temporal -> %s\n", temporal);

	while (p1 > 0) {
		while((p1 & 1) == 0) {
			p1 >>= 1;

			sube(&elemento->b1, &elemento->c1);
		}

		r3 = elemento->r1 - elemento->b1;

		if(p1 == 1 || compara(elemento->cadena[r3], temporal) < 0) p1 = 0;		
        else{
			p1--;

			if (elemento->b1 == 1){
				elemento->cadena[elemento->r1] = elemento->cadena[r3];
				printf("elemento->cadena[elemento->r1] -> %s\n", elemento->cadena[elemento->r1]);
				elemento->r1 = r3;
			} 
            else if (elemento->b1 >= 3){
				r2 = elemento->r1 - elemento->b1 + elemento->c1;

				if(compara(elemento->cadena[elemento->r1 - 1], elemento->cadena[r2]) >= 0){
					printf("elemento->cadena[elemento->r1-1] -> %s\n", elemento->cadena[elemento->r1-1]);
					printf("elemento->cadena[r2] -> %s\n", elemento->cadena[r2]);
					r2 = elemento->r1 - 1;

				    baja(&elemento->b1, &elemento->c1);

					p1 = p1 << 1;
				}

				if(compara(elemento->cadena[r2], elemento->cadena[r3]) < 0) {
					elemento->cadena[elemento->r1] = elemento->cadena[r3];
					printf("elemento->cadena[r2] -> %s\n", elemento->cadena[r2]);
					printf("elemento->cadena[r3] -> %s\n", elemento->cadena[r3]);
					elemento->r1 = r3;
				}
                else {
					elemento->cadena[elemento->r1] = elemento->cadena[r2];
					printf("elemento->cadena[elemento->r1] -> %s\n", elemento->cadena[elemento->r1]);
					elemento->r1 = r2;

					baja(&elemento->b1, &elemento->c1);

					p1 = 0;
				}
			}
		}
	}

	if (elemento->r1 - r0 != 0) {
		elemento->cadena[elemento->r1] = temporal;
		printf("elemento->cadena[elemento->r1] -> %s\n", elemento->cadena[elemento->r1]);
	}

	sift(elemento);
}

void semitrinkle(struct estado *elemento){
	palabra temporal;
	printf("\n--------------SEMITRINKLE-----------\n");

	elemento->r1 = elemento->r - elemento->c;

	if (compara(elemento->cadena[elemento->r1], elemento->cadena[elemento->r]) >= 0) {
		temporal = elemento->cadena[elemento->r];
		printf("temporal -> %s\n", temporal);
		elemento->cadena[elemento->r] = elemento->cadena[elemento->r1];
		printf("elemento->cadena[elemento->r] -> %s\n", elemento->cadena[elemento->r]);
		elemento->cadena[elemento->r1] = temporal;
		printf("elemento->cadena[elemento->r1] -> %s\n", elemento->cadena[elemento->r1]);

		trinkle(elemento);
	}
}
