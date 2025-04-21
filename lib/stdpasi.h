/*  stdpasi.h
    by Alanpasi (21/08/2023)

    Contém:
    Declarações:
        Macros
        Estruturas de dados
        Protótipos de funções
        Variáveis ou constantes globais externas
*/

#ifndef _STDPASI_H
#define _STDPASI_H

#define ONE_DAY_IN_SEC 86400 // 1 dia em segundos => 60 * 60 * 24

/*  daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957-03-04", "2023-07-23");
*/
int daysbtd(char *date1, char *date2);

#endif
