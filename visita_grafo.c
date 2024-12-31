#include "visita_grafo.h"

/* Calcola e stampa il numero massimo di giorni che ci vogliono per passare il contagio fra qualunque coppia di individui e il nome dei primi due individui in ordine alfabetico che rispettano questo numero di giorni */
void diametro_popolazione (Individuo_array* individui)
{
  uint *C; /* array che segna la distanza minima di un individuo da tutti gli altri */
  uint max, new_max; /* tengono traccia del massimo di giorni */
  uint inizio, fine;
  uint i, k, l; /* indici per i cicli for */

  max = 0;
  inizio = 0;
  fine = 0;

  C = calloc(individui->lunghezza,sizeof(uint));

  /* Per ogni individuo */
  for (i=0; i<individui->lunghezza; i++)
  {

    /* Setta la sua distanza dagli altri individui ad un valore massimo */
    for (k=0; k<individui->lunghezza; k++)
    {
      C[k]=individui->lunghezza;
    }

    /* Calcola la distanza minima dell'individuo da tutti gli altri e la segna nell'array C */
    visita_grafo_ampiezza(individui, i, C);

    /* Calcola il massimo valore dell'array C */
    new_max = max_giorni(C, individui);

    /* Se è più grande dei precedenti */
    if (max < new_max)
    {
      /* Segnali come nuovo massimo e tieni traccia dei due individui a distanza massima */
      max = new_max;

      inizio = i;

      for(l = 0; C[l] != max; l++);
      fine = l;
    }
  }

  /* Stampa il massimo di giorni e i primi due individui in ordine alfabetico che sono a distanza massima */
  printf("%u giorni: %s %s\n", max, individui->p[inizio].nome, individui->p[fine].nome);

  free(C);
}

/* Visita in ampiezza il grafo G a partire dalla sorgente s marcando i vertici visitati in C con la distanza da s */
void visita_grafo_ampiezza (Individuo_array *individui, uint s, uint *C)
{
  Coda_contagio coda;
  uint v, j, w;


  /* Crea una coda vuota */
  coda_contagio_new(&coda, individui->lunghezza);

  /* Marca la sorgente s e la inserisce nella coda */
  C[s] = 0;
  coda_contagio_enqueue(s,&coda);

  /* Finche' la coda non e' vuota */
  while (!coda_contagio_vuota_controllo(&coda))
  {
    /* Estrae l'elemento in testa alla coda */
    v = coda_contagio_head(&coda);
    coda_contagio_dequeue(&coda);

    /* Scorre i vertici adiacenti a tale elemento */
    for (j=0; j<individui->p[v].contatti.lunghezza; j++)
    {
      /* Se l'elemento adiacente non e' ancora stato visitato */
      w = individui->p[v].contatti.p[j].id;
      if (C[w] == individui->lunghezza )
      {
        /* Marca l'elemento adiacente e lo inserisce nella coda */
        C[w] = C[v]+1;
        coda_contagio_enqueue(w,&coda);
      }
    }
  }

  coda_contagio_elimina(&coda);
}

/* Restituisce il massimo fra gli elementi di un vettore e aggiorna l'indice in cui si trova il massimo*/
uint max_giorni (uint *C, Individuo_array *individui)
{
  uint i;
  uint max;

  max = 0;

  for (i=0; i<individui->lunghezza; i++)
  {
    if (C[i] > max)
    {
      max = C[i];
    }
  }

  return max;
}

/* Crea una coda vuota */
void coda_contagio_new (Coda_contagio *coda_contagio, uint size)
{
  coda_contagio->V = (uint *) calloc(size,sizeof(uint));
  if (coda_contagio->V == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della coda!\n");
    exit(EXIT_FAILURE);
  }

  coda_contagio->size = size;
  coda_contagio->head = 0;
  coda_contagio->tail = 0;
}

/* Elimina una coda esistente */
void coda_contagio_elimina (Coda_contagio *coda_contagio)
{
  free(coda_contagio->V);
  coda_contagio->V = NULL;
  coda_contagio->size = 0;
  coda_contagio->head = 0;
  coda_contagio->tail = 0;
}

/* Controlla se la coda contiene elementi */
boolean coda_contagio_vuota_controllo (Coda_contagio *coda_contagio)
{
  return (coda_contagio->head == coda_contagio->tail);
}

/* Restituisce la testa della coda */
uint coda_contagio_head (Coda_contagio *coda_contagio)
{
  return coda_contagio->V[coda_contagio->head];
}


/* Inserisce un elemento in coda e sposta di uno l'indice della coda*/
void coda_contagio_enqueue (uint i, Coda_contagio *coda_contagio)
{
  coda_contagio->V[coda_contagio->tail] = i;
  coda_contagio->tail++;

  if (coda_contagio->tail >= coda_contagio->size) coda_contagio->tail -= coda_contagio->size;
}

/* Sposta di uno l'indice della testa (eliminando di fatto il primo elemento) */
void coda_contagio_dequeue (Coda_contagio *coda_contagio)
{
  coda_contagio->head++;

  if (coda_contagio->head >= coda_contagio->size) coda_contagio->head -= coda_contagio->size;
}
