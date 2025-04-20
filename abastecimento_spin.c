#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define NUM_RECORD  200

/* struct da Nota */
typedef struct {
    char date[NUM_RECORD][11];  /* [ROW][COLUMN] => [Number of Items] [Size of Item] */
    int odometer[NUM_RECORD];
    double unit_price[NUM_RECORD];
    double liters[NUM_RECORD];
    double total_amount[NUM_RECORD];
} Nota;

/* (Similar a um CONSTRUCTOR) Função para criar uma Nota  */
Nota *criar_nota() {
    Nota *nova_nota = (Nota *) malloc(sizeof(Nota));
    if (nova_nota == NULL) {
        perror("Falha ao alocar memória para 'nova_nota'");
    }
    return nova_nota;
}

/* (Similar a um METHOD) Função que calcula a quantidade de quilômetros */
int total_odometer(Nota nota, int i) {
    return nota.odometer[i] - nota.odometer[0];
}

/* (Similar a um METHOD) Função que calcula o total_amount */
double total_amount(double liters, double unit_price) {
    return liters * unit_price;
}

int main(void) {

    struct stat st;

    char *pstr;
    char line[BUFSIZ];  /* Buffer para acumular uma linha lida */

    int i = 0;  /* Armazena o número de linhas lidas do arquivo */

    char *file = "abastecimento_spin.csv";  /* String do nome do arquivo */

    if (stat(file, &st) == 0) {
        printf("file_size = %ld\n", st.st_size);    /* Obtém o tamanho do arquivo */
        printf("Number of records = %ld\n", (st.st_size / 36) - 1); /* Calcula o número de linhas */
    }

    FILE *fstr = fopen(file, "r");  /* Abre arquivo para leitura ("r") */
    if (!fstr) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    Nota *nota = criar_nota();

    /* Coleta todas as notas */
    while (fgets(line, BUFSIZ, fstr) != NULL) {
        pstr = strtok(line, ",");
        strcpy(nota->date[i], pstr);
        pstr = strtok(NULL, ",");
        nota->odometer[i] = atoi(pstr);
        pstr = strtok(NULL, ",");
        nota->unit_price[i] = atof(pstr);
        pstr = strtok(NULL, ",");
        nota->liters[i] = atof(pstr);
        pstr = strtok(NULL, ",");
        nota->total_amount[i] = atof(pstr);
        i++;
    }

    printf("Initial date = %s, Final date = %s\n", nota->date[0], nota->date[i - 1]);

    printf("Initial odometer = %d, Final odometer = %d\n", nota->odometer[0], nota->odometer[i - 1]);
    int odometer = total_odometer(*nota, i - 1);
    printf("Total odometer= %d\n", odometer);

    /* Libera memória alocada */
    free(nota);
    fclose(fstr);

    return 0;
}
