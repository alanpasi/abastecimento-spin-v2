#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "lib/stdpasi.h"

#define NUM_RECORD  200

/* struct da Invoice */
// typedef struct {
//     char date[NUM_RECORD][11];  /* [ROW][COLUMN] => [Number of Items] [Size of Item] */
//     int odometer[NUM_RECORD];
//     double unit_price[NUM_RECORD];
//     double liters[NUM_RECORD];
//     double total_amount[NUM_RECORD];
// } Invoice;

/* Prototypes */
int total_odometer(Invoice invoice, int records);
double total_amount(double liters, double unit_price);
double final_amount(Invoice *invoice, int records);

int main(void) {

    struct stat st;

    FILE *fstr;

    int i = 0;  /* Armazena o número de linhas lidas do arquivo (index) */
    char *pstr;
    char line[BUFSIZ];  /* Buffer para acumular uma linha lida */
    char *file = "abastecimento_spin.csv";  /* String do nome do arquivo */

    if (stat(file, &st) == 0) {
        printf("file_size = %ld bytes\n", st.st_size);    /* Obtém o tamanho do arquivo */
        printf("Number of records = %ld\n", (st.st_size / 36) - 1); /* Calcula o número de linhas */
    }

    fstr = fopen(file, "r");  /* Abre arquivo para leitura ("r") */
    if (!fstr) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    Invoice *invoice = get_invoice_data();

    /* Coleta todas as notas */
    while (fgets(line, BUFSIZ, fstr) != NULL) {
        pstr = strtok(line, ",");
        strcpy(invoice->date[i], pstr);
        pstr = strtok(NULL, ",");
        invoice->odometer[i] = atoi(pstr);
        pstr = strtok(NULL, ",");
        invoice->unit_price[i] = atof(pstr);
        pstr = strtok(NULL, ",");
        invoice->liters[i] = atof(pstr);
        pstr = strtok(NULL, ",");
        invoice->total_amount[i] = atof(pstr);
        i++;
    }

    printf("Initial date = %s, Final date = %s\n", invoice->date[0], invoice->date[i - 1]);

    printf("Initial odometer = %d km, Final odometer = %d km\n", invoice->odometer[0], invoice->odometer[i - 1]);
    int odometer = total_odometer(*invoice, i - 1);
    printf("Total odometer= %d km\n", odometer);

    double total_amount = final_amount(invoice, i);
    printf("Total Amount = R$ %.2f\n", total_amount);

    char *date1 = invoice->date[0];
    char *date2 = invoice->date[i - 1];

    int total_days = daysbtd(date1, date2);
    printf("Total days = %d\n", total_days);

    /* Libera memória alocada */
    free(invoice);
    fclose(fstr);

    return 0;
}

/* Functions */

/* (Similar a um METHOD) Função que calcula a quantidade de quilômetros */
int total_odometer(Invoice invoice, int records) {
    return invoice.odometer[records] - invoice.odometer[0];
}

/* (Similar a um METHOD) Função que calcula o total_amount */
double total_amount(double liters, double unit_price) {
    return liters * unit_price;
}

/* */
double final_amount(Invoice *invoice, int records) {
    double res = 0;
    for (int r = 0; r < records; r++) {
        res = res + invoice->total_amount[r];
    }
    return res;
}
