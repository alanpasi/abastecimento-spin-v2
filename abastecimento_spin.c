#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "lib/stdpasi.h"

#define NUM_RECORD  200

int main(void) {

    struct stat st;
    Invoice *invoice;
    int records;
    char *file = "abastecimento_spin.csv";  /* String do nome do arquivo */

    if (stat(file, &st) == 0) {
        printf("file_size = %ld bytes\n", st.st_size);    /* Obtém o tamanho do arquivo */
        printf("Number of records = %ld\n", (st.st_size / 36) - 1); /* Calcula o número de linhas */
    }

    invoice = get_invoice_data(file, &records);

    printf("Initial date = %s, Final date = %s\n", invoice->date[0], invoice->date[records]);

    printf("Initial odometer = %d km, Final odometer = %d km\n", invoice->odometer[0], invoice->odometer[records]);
    int odometer = total_odometer(*invoice, records);
    printf("Total odometer= %d km\n", odometer);

    double amount = total_amount(invoice, records);
    printf("Total Amount = R$ %.2f\n", amount);

    char *date1 = invoice->date[0];
    char *date2 = invoice->date[records];

    int total_days = daysbtd(date1, date2);
    printf("Total days = %d\n", total_days);

    /* Libera memória alocada */
    free(invoice);

    return 0;
}
