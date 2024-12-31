/*TODO gcc -lm -std=c89 -Wall -Wextra -pedantic *.c *.h*/

#include "utilities.h"
#include "pandemia.h"
#include "individuo_array.h"
#include "visita_grafo.h"
#include "simulazione.h"

int main (int argc, char *argv[])
{
    FILE *file;
    Pandemia pandemia;
    Individuo_array individui;

    /* Legge dal file passato da riga di comando i dati riguardanti la simulazione e li salva nelle apposite strutture dati */
    assert(argc == 2);
    file = fopen(argv[1], "r");
    simulazione_new(file, &pandemia, &individui);
    fclose(file);

    /* Crea e stampa gli elenchi di individui ordinati in base all'età e in base alla refrattarietà */
    simulazione_stampa_elenchi(&individui);

    /* Descrive la potenziale velocità di diffusione dell'epidemia */
    diametro_popolazione(&individui);

    /* Crea e stampa la simulazione del contagio e i relativi dati statistici */
    simulazione_stampa(&pandemia,&individui);

    individuo_array_distruggi(&individui);

    return EXIT_SUCCESS;
}

