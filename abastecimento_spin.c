#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <locale.h>

#include "stdpasi.h"

int main(void) {

    /* Define o locale para "pt_BR" (português do Brasil) */
    // setlocale(LC_ALL, "pt_BR.UTF8");

    // Aloca memória para struct Invoice
    Invoice *invoice_data = malloc(sizeof(Invoice));
    if (invoice_data == NULL) {
        fprintf(stderr, "Falha ao alocar memória para struct Invoice");
        return 1;
    }
    invoice_data->record_count = 0; // Inicializa contador de registros

    printf("Call function read_db()!!!\n");
    read_db(invoice_data);

    listInvoiceData(invoice_data);

    /* Libera memória alocada */
    free(invoice_data);

    return 0;
}
