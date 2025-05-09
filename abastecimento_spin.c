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

    printf("Call function read_db()!!!\n");
    read_db(invoice_data);
    printf("Após read_db(), invoice_data->record_count = %d\n", invoice_data->record_count);

    listInvoiceData(invoice_data);

    resume(invoice_data, invoice_data->record_count);

    /* Libera memória alocada */
    free(invoice_data);

    return 0;
}
