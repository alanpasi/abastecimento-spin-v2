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
        Retorna (int) o número de dias entre duas datas (char) separadas
         por '-' Ex.: 2025-04-20.
*/
int daysbtd(char *pdate1, char *pdate2) {
    struct tm d1, d2;
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

/* (Similar a um CONSTRUCTOR) Função
    Cria uma Invoice
*/
Invoice *get_invoice_data(char *file, int *records) {
    Invoice *new_invoice = (Invoice *) malloc(sizeof(Invoice));
    if (new_invoice == NULL) {
        perror("Falha ao alocar memória para 'nova_nota'");
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
    *records = i - 1;   /* Define índice [0] a [36] => número de linhas '-1' */

    fclose(fstr);   /* Fecha o arquivo */

    return new_invoice;
}
