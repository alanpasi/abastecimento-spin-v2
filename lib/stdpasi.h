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

#define NUM_RECORD  200

/* struct da Invoice */
typedef struct {
    char date[NUM_RECORD][11];  /* [ROW][COLUMN] => [Number of Items] [Size of Item] */
    int odometer[NUM_RECORD];
    double unit_price[NUM_RECORD];
    double liters[NUM_RECORD];
    double total_amount[NUM_RECORD];
} Invoice;

/*  daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957-03-04", "2023-07-23");
*/
int daysbtd(char *date1, char *date2);

/* (Similar a um CONSTRUCTOR) Função
    Cria uma Invoice
*/
Invoice *get_invoice_data(char *file, int *records);

#endif
