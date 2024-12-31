#include "simulazione.h"

/* Legge dal file passato da riga di comando i dati riguardanti la simulazione e li salva nelle apposite strutture dati */
void simulazione_new(FILE * file, Pandemia *pandemia, Individuo_array *individui)
{
    /* Legge dal file i dati riguardanti la pandemia */
    pandemia_new(file, pandemia);

    /* Legge dal file i dati delle singole persone e crea un array che li immagazzina  */
    individuo_array_new(file, pandemia, individui);
}

/* Stampa gli elenchi di individui ordinati in base all'età e in base alla refrattarietà */
void simulazione_stampa_elenchi(Individuo_array *individui)
{
    /* Crea una copia di individui per evitare di modificare l'ordine alfabetico originale */
    Individuo_array copia;
    copia = individuo_array_copia(individui);

    /* Oridina e stampa l'array in base all'età (ordine decrescente), in caso di parità in base allo stato di salute (ordine crescente), in caso di ulteriore parità in base all'ordine alfabetico */
    quick_sort_anni(&copia, 0, copia.lunghezza - 1);
    individuo_array_stampa_ordine_anni(&copia);

    /* Oridino e stampo l'array in base alla refrattarietà dell'individuo (ordine crescente), in caso di parità in base all'ordine alfabetico */
    quick_sort_refrattarieta(&copia, 0, copia.lunghezza-1);
    individuo_array_stampa_ordine_refrattarieta(&copia);

    /* Elimina la copia dell'array */
    individuo_array_distruggi_copia(&copia);
}

/* Definizione delle procedure secondarie */
/* Simula e stampa l'andamento del contagio per ogni individuo durante i giorni della durata della pandemia. Legenda:
  - ’.’ per individui non infettivi (cio`e sani, guariti o appena contagiati il giorno stesso)
  - ’I’ per individui infettivi e non rilevati
  - ’O’ per individui ricoverati in ospedale
  - ’Q’ per individui in quarantena domestica*/
void simulazione_stampa(Pandemia *pandemia, Individuo_array *individui)
{
    uint giorno; /* Indica il giorno attuale  */
    uint i, j; /* Variabili per cicli for */
    uint n; /* Numero di colonne della matrice */
    uint letti_disponibili; /* Indica il numero di letti disponibili */
    char *stato_attuale; /* indica lo stato attuale di ogni individuo in ogni giornata della pandemia. Legenda '.':sano, 'I':infettivo, 'Q':coda o quarantena, 'O':ospedale */
    Individuo_array coda_ricovero; /* Priority queue per l'accesso all'ospedale */
    Pandemia_stat stat = {0};

    n = pandemia->n_giorni;

    stato_attuale = calloc(individui->lunghezza*(pandemia->n_giorni),sizeof(char));

    letti_disponibili = pandemia->letti_ospedale;

    /* Setta lo stato iniziale di ogni individuo nel giorno zero */
    for (i=0; i<individui->lunghezza; i++)
    {
      /* Se non è ancora stato contagiato */
      if (individui->p[i].giorni_contagio == 0)
      {
        /* Lo segno come sano */
        stato_attuale[i*n+0] = '.';
      }
      /* Altrimenti */
      else
      {
        /* Lo segno come infetto */
        stato_attuale[i*n+0] = 'I';
        /* Aumento il numero di giorni del contagio */
        individui->p[i].giorni_contagio++;
        /* Segno che un nuovo individuo è stato contagiato */
        stat.contagiati++;
      }
    }

    coda_ricovero.p = calloc(individui->lunghezza, sizeof *coda_ricovero.p);
    coda_ricovero.lunghezza = 0;

    /* Ogni giorno bisogna seguire questa procedura */
    for (giorno=0; giorno<pandemia->n_giorni; giorno++)
    {
      /* Dimette gli individui sani dall'ospedale. Se il numero di giorni trascorsi in quarantena più due volte quello dei giorni trascorsi in ospedale supera strettamente una determinata soglia un individuo viene dimesso o liberato dalla quarantena fin dalla mattina stessa */
      letti_disponibili += dimissioni(individui, pandemia, stato_attuale, giorno);

      /* Aggiorna la coda di priorità inserendo gli individui infetti e rilevati, tutti gli individui in coda vengono segnati in quarantena */
      aggiorna_coda(individui, &coda_ricovero, pandemia, stato_attuale, giorno);

      /* Finché ci sono letti disponibili */
      if (letti_disponibili > 0 && coda_ricovero.lunghezza > 0)
      {
        for (j=letti_disponibili; j && coda_ricovero.lunghezza > 0; j--)
        {
          /* Prende il primo individuo della coda, se non è già guarito ricoveralo in ospedale e aggiorna il suo stato*/
          if(ricovera(&coda_ricovero, individui, stato_attuale, pandemia, &stat, giorno, n))
          {
            letti_disponibili--;
          }
        }
      }

      /* Valuta i nuovi contagiati della giornata e crea uno stato provvisorio per il giorno dopo a vutti gli individui */
      if (giorno < pandemia->n_giorni - 1)
      {
        nuovi_contagi(individui, stato_attuale, &stat, giorno, n);
      }
    }

    /* Per ogni individuo */
    for (j=0; j < individui->lunghezza; j++)
    {
      /* Stampa l'andamento del contagio */
      printf("%-15s", &individui->p[j].nome);
      printf("%.*s", pandemia->n_giorni, &stato_attuale[j*n]);
      printf("\n");
    }

    printf("\n");

    /* Stampa le informazioni statistiche riguardanti la pandemia */
    stampa_statistica(stato_attuale, individui, pandemia, &stat);

    /* Libera la memoria allocata */
    individuo_array_distruggi_copia(&coda_ricovero);
    free(stato_attuale);
    stato_attuale = NULL;
}

/* Si dimettono gli individui sani dall'ospedale Se il numero di giorni trascorsi in quarantena più due volte quello dei giorni trascorsi in ospedale supera strettamente una determinata soglia un individuo viene dimesso o liberato dalla quarantena fin dalla mattina stessa */
uint dimissioni(Individuo_array *individui, Pandemia *pandemia, char *stato_attuale, uint giorno)
{
  uint i; /* Serve per indice for */
  uint n; /* Numero di colonne della matrice */
  uint result; /* Il numero di letti che vengono liberati */

  result = 0;

  n = pandemia->n_giorni;

  /* Per ogni individuo */
  for (i=0; i<individui->lunghezza; i++)
  {

    /* Se supera la soglia stabilita */
    if((double) individui->p[i].giorni_contagio - pandemia->giorni_incubazione - 1 > (double)(pandemia->giorni_convalescenza * (1-individuo_refrattarieta(&individui->p[i]))))
    {
      /* Se era in ospedale libera il letto */
      if(stato_attuale[i*n + giorno] == 'O')
      {
        result++;
      }

      /* Aggiorna lo stato attuale su guarito */
      stato_attuale[i*n + giorno] = '.';
    }
  }

  return result;
}

/* Valuta quali individui devono essere inseriti all'interno della coda e li inserisce*/
void aggiorna_coda(Individuo_array *individui, Individuo_array *coda_ricovero, Pandemia *pandemia, char *stato_attuale, uint giorno)
{
    uint i; /* Variabile cicli for */
    uint n; /* Numero delle colonne */

    n = pandemia->n_giorni;

    /* Per ogni individuo */
    for(i=0; i<individui->lunghezza; i++)
    {
      /* Se sono passati i giorni di incubazione */
      if (individui->p[i].giorni_contagio - 1 == pandemia->giorni_incubazione)
      {
        /* l'individuo viene rilevato ed inserito nella coda e il suo stato provvisorio passa a 'Q' */
        stato_attuale[i*n + giorno] = 'Q';
        coda_ricovero_aggiungi (coda_ricovero, &individui->p[i]);
      }
    }
}

/* TODO Prende il primo individuo della coda, lo ricovera in ospedale e aggiorna il suo stato*/
boolean ricovera(Individuo_array *coda_ricovero, Individuo_array *individui, char *stato_attuale, Pandemia *pandemia, Pandemia_stat* stat, uint giorno, uint n)
{
    Individuo ricoverato;
    uint indice; /* Indice della persona ricoverata */

    /* Finché ci sono individui in coda */
    while (coda_ricovero->lunghezza>0)
    {
      /* Toglie il primo individuo della coda_ricovero */
      ricoverato = coda_ricovero_prendi(coda_ricovero);

      /* Ricavati il suo indice nell'array individuo */
      indice = individuo_array_ricerca (ricoverato.nome, individui);

      /* Se non è ancora guarito */
      if ((double) individui->p[indice].giorni_contagio - pandemia->giorni_incubazione - 1 <= (double)(pandemia->giorni_convalescenza * (1-individuo_refrattarieta(&individui->p[indice]))))
      {
        /* Modifica il suo stato attuale e incrementa di uno l'informazione statistica degli individui ricoverati */
        stato_attuale [indice*n + giorno] = 'O';
        stat->ricoverati++;
        return TRUE;
      }
    }

    return FALSE;
}

/* Riempie lo stato di ogni individuo del giorno dopo con uno stato provvisorio, indicando se ci sono nuovi individui contagiati */
void nuovi_contagi(Individuo_array *individui, char *stato_attuale, Pandemia_stat* stat, uint giorno, uint n)
{
    uint i, j; /* Indici per i cicli for */
    uint id; /* Indice in cui si trovano i contatti */

    double somma; /* somma delle frequenze con i contatti infetti */

    /* Per ogni individuo */
    for (i=0; i<individui->lunghezza; i++)
    {
      somma = 0;

      /* Se è ancora sano */
      if (stato_attuale[i*n + giorno] == '.' && individui->p[i].giorni_contagio == 0)
      {
        /* Controllo i suoi contatti con gli individui infetti */
        for (j=0; j<individui->p[i].numero_contatti; j++)
        {
          id = individui->p[i].contatti.p[j].id;

          if (stato_attuale[id*n + giorno] == 'I')
          {
            somma += individui->p[i].contatti.p[j].frequenza;
          }
        }

        /* Se la somma delle frequenze dei contatti con gli individui infetti supera la refrattarietà dell'individuo */
        if (somma > individuo_refrattarieta(&individui->p[i]))
        {

          /* L'individuo sarà infetto a partire dal giorno dopo */
          stato_attuale[i*n + (giorno + 1)] = 'I';
          individui->p[i].giorni_contagio++;
          stat->contagiati++;
        }
        else
        {
          /* Altrimenti rimarrà sano */
          stato_attuale[i*n + (giorno + 1)] = '.';
        }
      }
      /* Se non è sano mantiene per il giorno dopo lo stato attuale in attesa di valutarlo, incrementando correttamente il numero di giorni contagio */
      else if (stato_attuale[i*n + giorno] == 'I')
      {
        stato_attuale[i*n + (giorno + 1)] = 'I';
        individui->p[i].giorni_contagio++;
      }

      else if (stato_attuale[i*n + giorno] == 'Q')
      {
        stato_attuale[i*n + (giorno + 1)] = 'Q';
        individui->p[i].giorni_contagio++;
      }

      else if (stato_attuale[i*n + giorno] == 'O')
      {
        stato_attuale[i*n + (giorno + 1)] = 'O';
        individui->p[i].giorni_contagio += 2;
      }
      else
      {
        stato_attuale[i*n + (giorno + 1)] = '.';
      }
    }
}

/* Stampa le informazioni statistiche riguardanti la pandemia */
void stampa_statistica(char *stato_attuale, Individuo_array *individui, Pandemia *pandemia, Pandemia_stat* stat)
{
    uint quarantena; /* Indica che l'individuo è stato almeno un giorno in quarantena */
    uint j; /* Indice per ciclo for */
    uint giorno; /* Indica il giorno della simulazione */
    uint n; /* numero di colonne dello stato salute */

    n = pandemia->n_giorni;

    /* Per ogni individuo */
    for (j=0; j < individui->lunghezza; j++)
    {
      quarantena = 0;
      /* Per ogni giorno */
      for (giorno = 0; giorno < pandemia->n_giorni; giorno++)
      {
        /* Se era in ospedale aumenta il dato statistico giorni_degenza */
        if (stato_attuale[j*n + giorno] == 'O')
        {
          stat->giorni_degenza++;
        }
        /* Se era in quarantena aumenta il dato statistico giorni_quarantena e la variabile quarantena */
        else if (stato_attuale[j*n + giorno] == 'Q')
        {
          stat->giorni_quarantena++;
          quarantena++;

        }
        /* Se l'ultimo giorno l'individuo non era sano aumenta il parametro statistico rimasti */
        if (giorno == pandemia->n_giorni-1 && stato_attuale[j*n + giorno] != '.')
        {
          stat->rimasti++;
        }
      }
      /* Se è stato almeno un giorno in quarantena aumenta il parametro statistico individui in attesa */
      if (quarantena != 0)
      {
        stat->individui_in_attesa++;
      }
    }

    /* Stampa tutti i dati statistici tranne l'ultimo */
    printf ("%u contagiati\n%u ricoverati\n%u rimasti\n%u individui in attesa\n%u giorni di degenza\n%u giorni di quarantena\n", stat->contagiati, stat->ricoverati, stat->rimasti, stat->individui_in_attesa, stat->giorni_degenza, stat->giorni_quarantena);

    /* Se c'è stato almeno un individuo in attesa calcola e stampa i giorni in attesa */
    if (stat->individui_in_attesa > 0)
    {
      stat->giorni_in_attesa = (double) stat->giorni_quarantena/stat->individui_in_attesa;

      printf ("%.3f giorni in attesa\n", stat->giorni_in_attesa);
    }
}


