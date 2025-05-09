#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "stdpasi.h"

int main(void) {

    // Aloca memória para struct Invoice
    Invoice *invoice_data = malloc(sizeof(Invoice));
    if (invoice_data == NULL) {
        fprintf(stderr, "Falha ao alocar memória para struct Invoice");
        return 1;
    }
    invoice_data->record_count = 0; // Inicializa contador de registros

    read_db(invoice_data);

    listInvoiceData(invoice_data);

    printf("Número de registros = %d\n", invoice_data->record_count);

    resume(invoice_data, invoice_data->record_count);

    /* Libera memória alocada */
    free(invoice_data);

    return 0;
}
