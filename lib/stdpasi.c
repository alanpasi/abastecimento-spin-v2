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
#include <locale.h>

#include <sqlite3.h>

#include "stdpasi.h"

/* Callback Function */
static int callback(void *arg, int argc, char *argv[], char *colName[]) {
    Invoice *current_invoice_data = (Invoice *)arg;
    if (current_invoice_data->record_count >= NUM_RECORD) {
        fprintf(stderr, "Aviso: Limite de registros (%d) atingido.\n", NUM_RECORD);
        return 1;
    }

    char temp_date[DATE_LEN] = "";
    unsigned int temp_odometer = 0;
    double temp_price = 0.0, temp_quantity = 0.0, temp_amount = 0.0;
    int data_found = 0;

    for (int i = 0; i < argc; i++) {
        if (argv[i]) {
            if (strcmp(colName[i], "date") == 0) {
                strncpy(temp_date, argv[i], DATE_LEN - 1);
                temp_date[DATE_LEN - 1] = '\0';
                data_found = 1;
            } else if (strcmp(colName[i], "odometer") == 0) {
                temp_odometer = atoi(argv[i]);
            } else if (strcmp(colName[i], "price") == 0) {
                temp_price = atof(argv[i]);
            } else if (strcmp(colName[i], "quantity") == 0) {
                temp_quantity = atof(argv[i]);
            } else if (strcmp(colName[i], "amount") == 0) {
                temp_amount = atof(argv[i]);
            }
        }
    }
    if (data_found) {
        strcpy(current_invoice_data->date[current_invoice_data->record_count], temp_date);
        current_invoice_data->odometer[current_invoice_data->record_count] = temp_odometer;
        current_invoice_data->unit_price[current_invoice_data->record_count] = temp_price;
        current_invoice_data->liters[current_invoice_data->record_count] = temp_quantity;
        current_invoice_data->total_amount[current_invoice_data->record_count] = temp_amount;
        current_invoice_data->record_count++;
    }

    return 0;
}

/*  Function
    daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957-03-04", "2023-07-23"); */
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

/* Function (Similar a um METHOD)
    Função que calcula a quantidade de quilômetros */
int total_odometer(Invoice invoice, int record_count) {
    return invoice.odometer[record_count - 1] - invoice.odometer[0];
}

/* Function
    Função que soma o Valor Total */
double total_amount(Invoice *invoice, int record_count) {
    double result = 0;
    for (int r = 0; r < record_count; r++) {
        result = result + invoice->total_amount[r];
    }
    return result;
}

/* Function
    Apresenta o resumo das Invoices */
void resume(Invoice *invoice, int record_count) {
    Resume result = {0};
    /* Coleta informações */
    strcpy(result.initial_date, invoice->date[0]);
    strcpy(result.final_date, invoice->date[record_count - 1]);

    char *date1 = result.initial_date;
    char *date2 = result.final_date;
    result.total_days = daysbtd(date1, date2);
    result.total_odometer = total_odometer(*invoice, record_count);
    result.total_amount = total_amount(invoice, record_count);
    result.total_liter = total_liters(invoice, record_count);
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
    Soma total de litros */
double total_liters(Invoice *invoice, int record_count) {
    double result = 0;
    for (int r = 0; r < record_count; r++) {
        result = result + invoice->liters[r];
    }
    return result;
}

/* Function
    Lê db */
int read_db(Invoice *invoice_data) {
    sqlite3 *db;
    int rc;
    char db_name[] = "./db/spin.sqlite";
    char *err_msg = 0;

    rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Mensagem: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else {
        fprintf(stdout, "db <%s> aberto com sucesso!\n", db_name);
    }

    // Create a SQL query
    char *sql_query = "SELECT * FROM notas";

    // Execute query
    rc = sqlite3_exec(db, sql_query, callback, invoice_data, &err_msg);


    /* Libera memória alocada */
    sqlite3_free(err_msg);
    sqlite3_close(db);

    return 0;
}

/* Function
    Lista invoices lidas do db */
void listInvoiceData(const Invoice *invoice_data) {
    setlocale(LC_ALL, "pt_BR.UTF8");
    for (int i = 0; i < invoice_data->record_count; i++) {
        printf("\nRegistro [%d] \n", i + 1);
        printf("Data: %s\n", invoice_data->date[i]);
        printf("Odometer: %d\n", invoice_data->odometer[i]);
        printf("Price: %.2f\n", invoice_data->unit_price[i]);
        printf("Quantity: %.2f\n", invoice_data->liters[i]);
        printf("Amount: %.2f\n", invoice_data->total_amount[i]);
    }
}
