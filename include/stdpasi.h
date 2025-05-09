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

#define NUM_RECORD  200 // Número de invoices
#define DATE_LEN 11 // 10 caracteres para data + 1 para o terminador nulo

/* struct da Invoice */
typedef struct {
    char date[NUM_RECORD][DATE_LEN];  /* [ROW][COLUMN] => [Number of Items] [Size of Item] */
    unsigned int odometer[NUM_RECORD];
    double unit_price[NUM_RECORD];
    double liters[NUM_RECORD];
    double total_amount[NUM_RECORD];
    unsigned int record_count;
} Invoice;

/* struct Resume */
typedef struct {
    char initial_date[DATE_LEN];
    char final_date[DATE_LEN];
    unsigned int total_days;
    unsigned int total_odometer;
    double total_amount;
    double total_liter;
    double amount_per_day;
    double amount_per_km;
    double km_per_day;
    double km_per_liter;
} Resume;

/*  daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957-03-04", "2023-07-23"); */
int daysbtd(char *date1, char *date2);

/* (Similar a um CONSTRUCTOR) Função
    Cria uma Invoice */
Invoice *get_invoice_data(char *file, int *records);

/* Function (Similar a um METHOD)
    Função que calcula a quantidade de quilômetros */
int total_odometer(Invoice invoice, int records);

/* Function
    Função que soma o Valor Total */
double total_amount(Invoice *invoice, int records);

/* Function
    Apresenta o resumo das Invoices */
void resume(Invoice *, int);

/* Function
    Soma total de litros */
double total_liters(Invoice *invoice, int records);

/* Function
    Inicializa db e retorna número de registros*/
int read_db(Invoice *);

/* Function
    Lista invoices lidas do db */
void listInvoiceData(const Invoice *);

#endif
