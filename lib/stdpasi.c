/*
    stdpasi.c
    by Alanpasi (21/08/2023)

    Contém:
        Definições das declarações do arquivo stdpasi.h
            Variáveis
            Constantes
            Funções

*/

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "stdpasi.h"

/*  daysbtd => DaysBetwenToDates
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
