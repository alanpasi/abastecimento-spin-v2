/*
    stdpasi.c
    by Alanpasi (21/08/2023)

    Contém:
        Definições das declarações do arquivo stdpasi.h
            Variáveis
            Constantes
            Funções
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "stdpasi.h"

/*  Function
    daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957-03-04", "2023-07-23");
*/
int daysbtd(char *pdate1, char *pdate2) {
    struct tm d1 = {0}, d2 = {0};
    int ndays;

    char *pstr;
    char date1[11], date2[11];

    strcpy(date1, pdate1);
    strcpy(date2, pdate2);

    pstr = strtok(date1, "-");
    d1.tm_year = atoi(pstr) - 1900;
    pstr = strtok(NULL, "-");
    d1.tm_mon = atoi(pstr) - 1;
    pstr = strtok(NULL, "-");
    d1.tm_mday = atoi(pstr);
    time_t date1_seconds = mktime(&d1);

    pstr = strtok(date2, "-");
    d2.tm_year = atoi(pstr) - 1900;
    pstr = strtok(NULL, "-");
    d2.tm_mon = atoi(pstr) - 1;
    pstr = strtok(NULL, "-");
    d2.tm_mday = atoi(pstr);
    time_t date2_seconds = mktime(&d2);

    ndays = (date2_seconds - date1_seconds) / ONE_DAY_IN_SEC;

    return ndays;
}

/* Function (Similar a um CONSTRUCTOR)
    Cria uma Invoice e coleta os dados do arquivo db (csv)
*/
Invoice *get_invoice_data(char *file, int *records) {
    Invoice *new_invoice = (Invoice *) malloc(sizeof(Invoice));
    if (new_invoice == NULL) {
        perror("Falha ao alocar memória para '*new_invoice'");
    }

    FILE *fstr;

    int i = 0;  /* Armazena o número de linhas lidas do arquivo (index) */
    char *pstr;
    char line[BUFSIZ];  /* Buffer para acumular uma linha lida */

    fstr = fopen(file, "r");  /* Abre arquivo para leitura ("r") */
    if (!fstr) {
        perror("fopen");
        return NULL;
    }

    /* Coleta todas as notas */
    while (fgets(line, BUFSIZ, fstr) != NULL) {
        pstr = strtok(line, ",");
        strcpy(new_invoice->date[i], pstr);
        pstr = strtok(NULL, ",");
        new_invoice->odometer[i] = atoi(pstr);
        pstr = strtok(NULL, ",");
        new_invoice->unit_price[i] = atof(pstr);
        pstr = strtok(NULL, ",");
        new_invoice->liters[i] = atof(pstr);
        pstr = strtok(NULL, ",");
        new_invoice->total_amount[i] = atof(pstr);
        i++;
    }
    *records = i - 1;   /* Define índice [0] a [i - 1] => número de linhas '-1' */

    fclose(fstr);   /* Fecha o arquivo */

    return new_invoice;
}

/* Function (Similar a um METHOD)
    Função que calcula a quantidade de quilômetros
*/
int total_odometer(Invoice invoice, int records) {
    return invoice.odometer[records] - invoice.odometer[0];
}

/* Function
    Função que soma o Valor Total
*/
double total_amount(Invoice *invoice, int records) {
    double result = 0;
    for (int r = 0; r < records; r++) {
        result = result + invoice->total_amount[r];
    }
    return result;
}

/* Function
    Apresenta o resumo das Invoices
*/
void resume(Invoice *invoice, int records) {
    Resume result;
    /* Coleta informações */
    strcpy(result.initial_date, invoice->date[0]);
    strcpy(result.final_date, invoice->date[records]);

    char *date1 = result.initial_date;
    char *date2 = result.final_date;
    result.total_days = daysbtd(date1, date2);
    result.total_odometer = total_odometer(*invoice, records);
    result.total_amount = total_amount(invoice, records);
    result.total_liter = total_liters(invoice, records);
    result.km_per_day = (double)result.total_odometer / result.total_days;
    result.amount_per_day = result.total_amount / result.total_days;
    result.amount_per_km = result.total_amount / result.total_odometer;
    result.km_per_liter = result.total_odometer / result.total_liter;

    puts("*** Resumo ***");
    printf("Date: %s to %s\n", result.initial_date, result.final_date);
    printf("Total days = %'d\n", result.total_days);
    printf("Total odometer = %'d km\n", result.total_odometer);
    printf("km per day = %'.1f\n", result.km_per_day);
    printf("Total amount = R$ %'.2f\n", result.total_amount);
    printf("Amount per day = R$ %'.2f\n", result.amount_per_day);
    printf("Amount per km = R$ %'.2f\n", result.amount_per_km);
    printf("km per liter = %.2f\n", result.km_per_liter);

}

/* Function
    Soma total de litros
*/
double total_liters(Invoice *invoice, int records) {
    double result = 0;
    for (int r = 0; r < records; r++) {
        result = result + invoice->liters[r];
    }
    return result;
}
