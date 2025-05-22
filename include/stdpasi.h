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

#define NUM_RECORD  400 // Número de invoices
#define DATE_LEN 11 // 10 caracteres para data + 1 para o terminador nulo

/* struct da Invoice */
typedef struct {
    char date[NUM_RECORD][DATE_LEN];  /* [ROW][COLUMN] => [Number of Items] [Size of Item] */
    unsigned odometer[NUM_RECORD];
    double unit_price[NUM_RECORD];
    double liters[NUM_RECORD];
    double total_amount[NUM_RECORD];
    unsigned record_count;
} Invoice;

/* struct InvoiceReport */
typedef struct {
    char initial_date[DATE_LEN];
    char final_date[DATE_LEN];
    unsigned total_days;
    unsigned total_odometer;
    double total_amount;
    double total_liter;
    double amount_per_day;
    double amount_per_km;
    double km_per_day;
    double km_per_liter;
} InvoiceReport;

/* Menu options */
typedef enum {
    INSERT = 1,
    LIST,
    REPORT,
    QUIT
} MenuOption;

/*  daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957-03-04", "2023-07-23"); */
int daysbtd(char *, char *);

/* Function (Similar a um METHOD)
    Função que calcula a quantidade de quilômetros */
int totalOdometer(Invoice *);

/* Function
    Função que soma o Valor Total */
double totalAmount(Invoice *);

/* Function
    Apresenta o resumo das Invoices */
InvoiceReport report(Invoice *);

/* Function
    Soma total de litros */
double totalLiters(Invoice *);

/* Function
    Inicializa db e retorna número de registros*/
int read_db(Invoice *);

/* Function
    Lista invoices lidas do db */
void listInvoiceData(Invoice *);

/* Function
    Cria a mainPage*/
int mainPage(Invoice *);

/* Function
    Adiciona invoice */
int addInvoice(Invoice *);

/* Function
    Save invoice */
int saveInvoice(Invoice *);

#endif
