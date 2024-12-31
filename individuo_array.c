#include "individuo_array.h"

/* Leggi dal file i dati delle singole persone e crea un array con i loro dati */
void individuo_array_new(FILE *file, Pandemia *pandemia, Individuo_array *individui)
{
    uint i; /* indici per i cicli for */


    /* Alloca la memoria necessaria per la variabile individui */
    individui->lunghezza = pandemia->n_individui;
    individui->p = calloc(individui->lunghezza, sizeof *individui->p);

    /* Crea un array con gli individui letti da file senza associare i contatti */
    for(i=0; i<pandemia->n_individui; i++)
    {
         individuo_new(file, &individui->p[i]);
    }


    /* Riordina l'array in ordine alfabetico */
    quick_sort_alfabetico(individui, 0, individui->lunghezza - 1);

    /* Riavvolge il file per poter leggere i contatti di ogni individuo */
    rewind(file);

    /* Scarta la prima riga */
    while (fgetc(file) != '\n');

    /* Associa ad ogni individuo i suoi contatti */
    individuo_array_associa_contatti(file, individui, pandemia);

}

/* Riordina l'array di individui in ordine alfabetico fra gli indici s e d con l'algoritmo QuickSort */
void quick_sort_alfabetico (Individuo_array *individui, uint s, uint d)
{
    uint q; /* indice dell'elemento pivot nel vettore individui ordinato */
    if (s < d)
    {
      q = partition_alfabetico(individui,s,d);
      quick_sort_alfabetico(individui,s,q);
      quick_sort_alfabetico(individui,q+1,d);
    }
}

/* Partiziona l'array di individui in due sottovettori sulla base dell'ordine alfabetico */
uint partition_alfabetico (Individuo_array *individui, uint s, uint d)
{
    uint d1; /* indice destro del primo sottovettore */
    uint i;  /* indice dell'elemento considerato via via */


    /* Il pivot e' V[s] e il primo sottovettore va da V[s] a V[s] */
    d1 = s;

    /* Scorre il vettore assegnando gli elementi via via al primo o secondo sottovettore */
    for (i = s+1; i <= d; i++)
      if (strcmp(individui->p[i].nome, individui->p[s].nome) <= 0)
      {
        d1++;
        scambia_individui(&individui->p[i], &individui->p[d1]);
      }

  /* Porta il pivot nella posizione finale del primo sottovettore e la restituisce */
    scambia_individui(&individui->p[s],&individui->p[d1]);

    return d1;
}

/* Scambia il posto di due individui all'interno dell'array */
void scambia_individui (Individuo *pa, Individuo *pb)
{
    Individuo temp;

    temp = *pa;
    *pa = *pb;
    *pb = temp;
}

/* Associa ad ogni individuo i suoi contatti */
void individuo_array_associa_contatti(FILE *file, Individuo_array *individui, Pandemia *pandemia)
{
    uint i, j; /* Indici dei cicli for */
    uint n; /* Numero di variabili lette da fscanf */
    string nome;
    uint id; /* indice del contatto */

    /* Per ogni riga */
    for (i=0; i<pandemia->n_individui; i++)
    {
      /* Legge il cognome all'inizio della riga */
      n = fscanf(file, "%s", nome);

      /* Controllo: fscanf deve leggere esattamente 1 parametro */
      assert(n == 1);

      /* Ricerca l'indice dell'array associato a quel cognome */
      id = individuo_array_ricerca(nome, individui);

      /* Salta 5 spazi all'interno della riga per ignorare i dati già presi */
      for (j=0; j<5; j++)
      {
        while (fgetc(file)!= ' ');
      }

      /* Crea l'array dei contatti all'individuo */
      individui->p[id].contatti = contatto_array_new(file, id, individui);
  }
}


/* Restituisce l'indice del punto dell'array dove si trova l'individuo */
uint individuo_array_ricerca(string nome, Individuo_array* individui)
{
    uint inizio, fine, i; /* Indici per la ricerca */
    int confronto;

    inizio = 0;
    fine = individui->lunghezza;

    /* Ricerca binaria del cognome all'interno dell'array */
    while (fine - inizio > 0)
    {
      i = inizio;
      i += (fine - inizio) / 2;
      confronto = strcmp(nome, individui->p[i].nome);
      if (confronto == 0)
      {
        return i;
      }
      else if (confronto < 0)
      {
        fine = i;
      }
      else
      {
        inizio = i + 1;
      }
    }

    /* Controllo: la ricerca deve sempre fornire un risultato */
    assert(0);

    return individui->lunghezza;
}

/* Crea l'array di contatti di un individuo */
Contatto_array contatto_array_new(FILE* file, uint id, Individuo_array* individui)
{
    uint i; /* Indice ciclo for */
    uint n; /* Numero di variabili lette da fscanf */
    Contatto_array contatti;
    string nome; /* cognome del contatto */

    contatti =  individui->p[id].contatti;
    contatti.lunghezza = individui->p[id].numero_contatti;
    contatti.p = calloc(contatti.lunghezza, sizeof *contatti.p);

    /* Per ogni contatto dell'individuo */
    for (i=0; i<contatti.lunghezza; i++)
    {
      /* Leggo i suoi dati e li inserisco nell'array dei contatti */
      n = fscanf(file, "%s %lg", nome, &contatti.p[i].frequenza);
      contatti.p[i].id = individuo_array_ricerca(nome, individui);

      /* Controllo: fscanf deve leggere esattamente 2 parametri */
      assert(n == 2);
    }

    return contatti;
}

/* Crea una copia di un array di individui */
Individuo_array individuo_array_copia(Individuo_array* individui)
{
    Individuo_array copia;
    uint i;

    copia.lunghezza = individui->lunghezza;
    copia.p = calloc(copia.lunghezza, sizeof *copia.p);

    for(i=0; i < copia.lunghezza; i++)
    {
      copia.p[i] = individui->p[i];
    }

    return copia;
}

/* Riordina l'array di individui in ordine di età decrescente fra gli indici s e d con l'algoritmo QuickSort, in caso di partità per stato di salute crescente; in caso di ulteriore parità, in ordine alfabetico */
void quick_sort_anni (Individuo_array *individui, uint s, uint d)
{
    uint q; /* indice dell'elemento pivot nel vettore individui ordinato */
    if (s < d)
    {
      q = partition_anni(individui,s,d);
      quick_sort_anni(individui,s,q);
      quick_sort_anni(individui,q+1,d);
    }
}

/* Partiziona l'array di individui in due sottovettori sulla base dell'ordine d'età */
uint partition_anni (Individuo_array *individui, uint s, uint d)
{
    uint d1; /* indice destro del primo sottovettore */
    uint i;  /* indice dell'elemento considerato via via */
    int cmp;  /* Mi da un numero negativo se p[i] viene prima di p[s] */

    d1 = s;

    /* Scorre il vettore assegnando gli elementi via via al primo o secondo sottovettore */
    for (i = s+1; i <= d; i++)
    {
      /* Controlla gli anni */
      cmp = individui->p[s].anni - individui->p[i].anni;

      /* Se sono uguali */
      if (cmp == 0)
      {
        /* Controlla lo stato di salute */
        cmp = (individui->p[i].salute > individui->p[s].salute) - (individui->p[i].salute < individui->p[s].salute);
      }
      /* Se anche questo è uguale */
      if (cmp == 0)
      {
        /* Controlla l'ordine alfabetico */
        cmp = strcmp(individui->p[i].nome, individui->p[s].nome);
      }
      /* Se non sono in ordine scambia i due individui */
      if (cmp <= 0)
      {
        d1++;
        scambia_individui(&individui->p[i], &individui->p[d1]);
      }
    }

    /* Porta il pivot nella posizione finale del primo sottovettore e la restituisce */
    scambia_individui(&individui->p[s],&individui->p[d1]);

    return d1;
}

/* Stampa gli individui in ordine per età */
void individuo_array_stampa_ordine_anni (Individuo_array* individui)
{
    uint i;

    printf("Ordine per eta':\n");

    /* Per ogni individuo */
    for (i=0; i<individui->lunghezza; i++)
    {
      /* stampa il cognome, gli anni e lo stato di salute (con 3 cifre decimali)*/
      printf("%s %u %.3f\n", individui->p[i].nome, individui->p[i].anni, individui->p[i].salute);
    }
}

/* Riordina l'array di individui in ordine di refrattarietà con l'algoritmo QuickSort  */
void quick_sort_refrattarieta (Individuo_array*individui, uint s, uint d)
{
    uint q; /* indice dell'elemento pivot nel vettore individui ordinato */

    if (s < d)
    {
      q = partition_refrattarieta(individui,s,d);
      quick_sort_refrattarieta(individui,s,q);
      quick_sort_refrattarieta(individui,q+1,d);
    }
}

/* U Partiziona l'array di individui in due sottovettori sulla base dell'ordine d'età */
uint partition_refrattarieta (Individuo_array* individui, uint s, uint d)
{
    uint d1; /* indice destro del primo sottovettore */
    uint i;  /* indice dell'elemento considerato via via */

    d1 = s;

    /* Scorre il vettore assegnando gli elementi via via al primo o secondo sottovettore */
    for (i = s+1; i <= d; i++)
    {
       /* Verifico refrattarietà individui e in caso li scambio */
      if (confronta_refrattarieta(&individui->p[i],&individui->p[s]) < 0)
      {
        d1++;
        scambia_individui(&individui->p[i],&individui->p[d1]);
      }
    }

    /* Porta il pivot nella posizione finale del primo sottovettore e la restituisce */
    scambia_individui(&individui->p[s],&individui->p[d1]);

    return d1;
}

/* U Stampa gli individui in ordine per refrattarietà */
void individuo_array_stampa_ordine_refrattarieta (Individuo_array *individui)
{
    uint i;


    printf("Ordine per refrattarieta':\n");

    for (i=0; i<individui->lunghezza; i++)
    {
        printf("%s %.3f\n", individui->p[i].nome, individuo_refrattarieta(&individui->p[i]));
    }
}

/* Distrugge un array di individui senza toccare i contatti */
void individuo_array_distruggi_copia(Individuo_array*individui)
{
    free(individui->p);
    individui->p = NULL;
    individui->lunghezza = 0;
}

/* Distrugge un array di individui */
void individuo_array_distruggi(Individuo_array* individui)
{
    uint i;

    for (i=0; i<individui->lunghezza; i++)
    {
      individuo_distruggi_contatti(&individui->p[i]);
    }

    individuo_array_distruggi_copia(individui);
}
