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

#include <sqlite3.h>

#include "stdpasi.h"

/* Callback Function
 * void *arg => Ponteiro da Invoice
 * int argc => Número de campos do db
 * char *argv[] => Valores de cada campo
 * char *colName[] => Nome de cada campo do db
*/
static int callback(void *arg, int argc, char *argv[], char *colName[]) {
    Invoice *invoice = (Invoice *)arg;
    if (invoice->record_count >= NUM_RECORD) {
        fprintf(stderr, "Aviso: Limite de registros (%d) atingido.\n", NUM_RECORD);
        return 1;
    }

    char temp_date[DATE_LEN] = "";
    unsigned int temp_odometer = 0;
    double temp_price = 0.0, temp_liters = 0.0, temp_amount = 0.0;
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
            } else if (strcmp(colName[i], "liters") == 0) {
                temp_liters = atof(argv[i]);
            } else if (strcmp(colName[i], "amount") == 0) {
                temp_amount = atof(argv[i]);
            }
        }
    }
    if (data_found) {
        strcpy(invoice->date[invoice->record_count], temp_date);
        invoice->odometer[invoice->record_count] = temp_odometer;
        invoice->unit_price[invoice->record_count] = temp_price;
        invoice->liters[invoice->record_count] = temp_liters;
        invoice->total_amount[invoice->record_count] = temp_amount;
        invoice->record_count++;
    }

    return 0;
}

/*  Function
    daysbtd => DaysBetwenToDates
    Retorna (int) o número de dias entre duas datas (char) separadas por '-'.
    Eg. daysbtd("1957/03/04", "2023/07/23"); */
int daysbtd(char *pdate1, char *pdate2) {
    struct tm d1 = {0}, d2 = {0};
    int ndays = 0;

    char *pstr = NULL;
    char date1[11], date2[11];

    strcpy(date1, pdate1);
    strcpy(date2, pdate2);

    pstr = strtok(date1, "/");
    d1.tm_year = atoi(pstr) - 1900;
    pstr = strtok(NULL, "/");
    d1.tm_mon = atoi(pstr) - 1;
    pstr = strtok(NULL, "/");
    d1.tm_mday = atoi(pstr);
    time_t date1_seconds = mktime(&d1);

    pstr = strtok(date2, "/");
    d2.tm_year = atoi(pstr) - 1900;
    pstr = strtok(NULL, "/");
    d2.tm_mon = atoi(pstr) - 1;
    pstr = strtok(NULL, "/");
    d2.tm_mday = atoi(pstr);
    time_t date2_seconds = mktime(&d2);

    ndays = (date2_seconds - date1_seconds) / ONE_DAY_IN_SEC;

    return ndays;
}

/* Function (Similar a um METHOD)
    Função que calcula a quantidade de quilômetros */
int total_odometer(Invoice *invoice) {
    return invoice->odometer[invoice->record_count - 1] - invoice->odometer[0];
}

/* Function
    Função que soma o Valor Total */
double total_amount(Invoice *invoice) {
    double result = 0;
    for (int r = 0; r < invoice->record_count; r++) {
        result = result + invoice->total_amount[r];
    }
    return result;
}

/* Function
    Apresenta o resumo das Invoices */
void resume(Invoice *invoice) {

    Resume resume = {0};
    // Coleta informações
    strcpy(resume.initial_date, invoice->date[0]);
    strcpy(resume.final_date, invoice->date[invoice->record_count - 1]);

    char *date1 = resume.initial_date;
    char *date2 = resume.final_date;
    resume.total_days = daysbtd(date1, date2);
    resume.total_odometer = total_odometer(invoice);
    resume.total_amount = total_amount(invoice);
    resume.total_liter = total_liters(invoice);
    resume.km_per_day = (double)resume.total_odometer / resume.total_days;
    resume.amount_per_day = resume.total_amount / resume.total_days;
    resume.amount_per_km = resume.total_amount / resume.total_odometer;
    resume.km_per_liter = resume.total_odometer / resume.total_liter;

    puts("*** Resumo ***");
    printf("Número de registros = %d\n", invoice->record_count);
    printf("Date: %s to %s\n", resume.initial_date, resume.final_date);
    printf("Total days = %'d\n", resume.total_days);
    printf("Total odometer = %'d km\n", resume.total_odometer);
    printf("km per day = %'.1f\n", resume.km_per_day);
    printf("Total amount = R$ %'.2f\n", resume.total_amount);
    printf("Amount per day = R$ %'.2f\n", resume.amount_per_day);
    printf("Amount per km = R$ %'.2f\n", resume.amount_per_km);
    printf("km per liter = %'.2f\n", resume.km_per_liter);
}

/* Function
    Soma total de litros */
double total_liters(Invoice *invoice) {
    double result = 0;
    for (int i = 0; i < invoice->record_count; i++) {
        result = result + invoice->liters[i];
    }
    return result;
}

/* Function
    Inicializa db e retorna número de registros*/
int read_db(Invoice *invoice) {
    sqlite3 *db;
    int rc = 0;
    char db_name[] = "./db/spin.sqlite";
    char *err_msg = NULL;
    char *sql_query = NULL;

    rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Mensagem: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else {
        fprintf(stdout, "db <%s> aberto com sucesso!\n", db_name);
    }

    // Create a SQL query
    sql_query = "SELECT * FROM notas";

    // Execute query
    rc = sqlite3_exec(db, sql_query, callback, invoice, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    // Libera memória alocada
    sqlite3_free(err_msg);
    sqlite3_close(db);

    return invoice->record_count;
}

/* Function
    Lista invoices lidas do db */
void listInvoiceData(Invoice *invoice) {

    for (int i = 0; i < invoice->record_count; i++) {
        printf("\nRegistro [%d] \n", i + 1);
        printf("Data: %s\n", invoice->date[i]);
        printf("Odometer: %'d km\n", invoice->odometer[i]);
        printf("Price: R$ %'.2f\n", invoice->unit_price[i]);
        printf("Liters: %'.1f L\n", invoice->liters[i]);
        printf("Amount: R$ %'.2f\n", invoice->total_amount[i]);
    }
}

/* Function
    Cria a mainPage*/
int mainPage(Invoice *invoice) {

    unsigned option = 0;

    // Sequência de escape ANSI para limpar a tela e posicionar o cursor no topo
    printf("\033[H\033[J");

    do {
        printf("========\033[30;41m Abastecimento da Spin \033[0m========\n");
        printf("\tNúmero de registros = %d\n", invoice->record_count);
        printf("\t1 - Inserir\n");
        printf("\t2 - Listar\n");
        printf("\t3 - Resumo\n");
        printf("\t4 - Sair\n");

        printf("Selecione uma opção: ");
        scanf("%d*c", &option);

        switch ((unsigned)option) {
            case INSERT:
                add_invoice(invoice);
                break;
            case LIST:
                listInvoiceData(invoice);
                break;
            case RESUME:
                resume(invoice);
                break;
            case QUIT:
                // TO_DO
                printf("**** QUIT ****\n");
                break;
            default:
            printf("Digite uma opção válida!!!\n");
        }
    } while (option != QUIT);

    return 0;
}

/* Function
    Adiciona invoice */
int add_invoice(Invoice *invoice) {
    sqlite3 *db;
    int rc = 0;
    char db_name[] = "./db/spin.sqlite";
    char *sql_query = NULL;
    sqlite3_stmt *stmt;

    // Dados a serem inseridos (usando variáveis)
    char date[DATE_LEN];
    unsigned int odometer = 0;
    double price = 0.0;
    double liters = 0.0;
    double amount = 0.0;

    rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Mensagem: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else {
        fprintf(stdout, "db <%s> aberto com sucesso!\n", db_name);
    }

    // Entrada de dados
    printf("\nEntre a data (ex. aaaa/mm/dd): ");
    scanf("%s*c", date);
    printf("Entre odometer (ex. 116000): ");
    scanf("%d", &odometer);
    printf("Entre o preço/litro (ex. 6,19): ");
    scanf("%lf", &price);
    printf("Entre o quantidade de litros (ex. 51,2): ");
    scanf("%lf", &liters);
    printf("Entre o valor da nota (ex. 312,42): ");
    scanf("%lf", &amount);

    // Create a SQL query
    sql_query = "INSERT INTO notas (date, odometer, price, liters, amount) VALUES (?, ?, ?, ?, ?);";

    // Preparar a declaração SQL
    rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a declaração: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Vincular os valores às variáveis
    sqlite3_bind_text(stmt, 1, date, strlen(date), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, odometer);
    sqlite3_bind_double(stmt, 3, price);
    sqlite3_bind_double(stmt, 4, liters);
    sqlite3_bind_double(stmt, 5, amount);

    // Executar a declaração
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erro ao executar a inserção: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Dados inseridos com sucesso\n");

        // Atualiza struct invoice
        strcpy(invoice->date[invoice->record_count], date);
        invoice->odometer[invoice->record_count] = odometer;
        invoice->unit_price[invoice->record_count] = price;
        invoice->liters[invoice->record_count] = liters;
        invoice->total_amount[invoice->record_count] = amount;
        invoice->record_count ++;

        fprintf(stdout, "Número de registros: %d\n",  invoice->record_count);
    }

    // Finalizar a declaração
    sqlite3_finalize(stmt);

    // Libera memória alocada
    sqlite3_close(db);

    return 0;
}

/* Function
    Save invoice */
int saveInvoice(Invoice *invoice) {
    sqlite3 *db;
    int rc = 0;
    char db_name[] = "./db/spin.sqlite";
    char *sql_query = NULL;
    sqlite3_stmt *stmt;

    rc = sqlite3_open(db_name, &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Mensagem: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    } else {
        fprintf(stdout, "db <%s> aberto com sucesso!\n", db_name);
    }

    // Create a SQL query
    sql_query = "INSERT INTO notas (date, odometer, price, liters, amount) VALUES (?, ?, ?, ?, ?);";

    // Preparar a declaração SQL
    rc = sqlite3_prepare_v2(db, sql_query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a declaração: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Vincular os valores às variáveis
    sqlite3_bind_text(stmt, 1, invoice->date[invoice->record_count],
                        strlen(invoice->date[invoice->record_count]),
                        SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, invoice->odometer[invoice->record_count]);
    sqlite3_bind_double(stmt, 3, invoice->unit_price[invoice->record_count]);
    sqlite3_bind_double(stmt, 4, invoice->liters[invoice->record_count]);
    sqlite3_bind_double(stmt, 5, invoice->total_amount[invoice->record_count]);

    // Executar a declaração
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erro ao executar a inserção: %s\n", sqlite3_errmsg(db));
    } else {
        fprintf(stdout, "Dados inseridos com sucesso\n");

        // Atualiza struct invoice
        // strcpy(invoice->date[invoice->record_count], date);
        // invoice->odometer[invoice->record_count] = odometer;
        // invoice->unit_price[invoice->record_count] = price;
        // invoice->liters[invoice->record_count] = liters;
        // invoice->total_amount[invoice->record_count] = amount;
        // invoice->record_count ++;

        fprintf(stdout, "Número de registros: %d\n",  invoice->record_count);
    }

    // Finalizar a declaração
    sqlite3_finalize(stmt);

    // Libera memória alocada
    sqlite3_close(db);

    return 0;
}
