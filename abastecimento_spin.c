#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <locale.h>
#include "lib/stdpasi.h"

#define NUM_RECORD  200

int main(void) {

    setlocale(LC_ALL, "pt_BR.UTF8");

    struct stat st;
    Invoice *invoice;
    int records;
    char *file = "abastecimento_spin.csv";  /* String do nome do arquivo */

    if (stat(file, &st) == 0) {
        printf("file_size = %'ld bytes\n", st.st_size);    /* Obtém o tamanho do arquivo */
        printf("Number of records = %'ld\n", (st.st_size / 36) - 1); /* Calcula o número de linhas */
    }

    invoice = get_invoice_data(file, &records);

    /* Mostra resumo geral */
    resume(invoice, records);

    /* Libera memória alocada */
    free(invoice);

    return 0;
}
