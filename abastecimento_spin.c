#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "init_gtk.h"
#include "stdpasi.h"

int main(int argc, char *argv[]) {

    // Aloca memória para struct Invoice
    Invoice *invoice = malloc(sizeof(Invoice));
    if (invoice == NULL) {
        fprintf(stderr, "Falha ao alocar memória para struct Invoice");
        return 1;
    }
    // Inicializa contador de registros
    invoice->record_count = 0;

    read_db(invoice);

    // Atenção!!! Usar a função setlocale() somente após a leitura dos dados no SQLite3.
    // Se usar antes, ocorre o truncamento do valor em campo 'REAL'
    setlocale(LC_ALL, "pt_BR.UTF8");

    // Inicia GtkApplicationWindow
    init_gtk(argc, argv);
    // mainPage(invoice);

    // Libera memória alocada
    free(invoice);

    return 0;
}
