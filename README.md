# Groupe de blanch_b 1003936
login: blanch_b loizag_g utzman_f baudet_c

## Requirements
- Linux (Tested on Debian 11)
- GCC compiler : `sudo apt-get install build-essential`
- Apache Bench : `sudo apt install apache2-utils`
    - Usage : `ab -n 1000 -c 100 http://localhost:8080/`
- Valgrind : `sudo apt-get install valgrind`
- GDB : `sudo apt-get install gdb`
- Criterion : https://github.com/Snaipe/Criterion#downloads 
    - Ubuntu (>=21.04) / Debian (>=11) : `sudo apt-get install libcriterion-dev`
    - Other : https://criterion.readthedocs.io/en/latest/setup.html#installation

## How to use
- Compile the program : `make`
- Run the program : `./cherokee`
- Clean the program : `make clean`
- Clean the program and the executable : `make fclean`
- Re-compile the program : `make re`
- Run the program with valgrind : `valgrind ./cherokee` => Debug mode (memory leaks)
- Run the program with gdb : `gdb ./cherokee` => Debug mode
<!-- - Run criterion tests : `make tests_run` -->
<!-- - Run functional tests : ... -->

## Notes
Oui, il est possible de limiter le nombre de threads pouvant exécuter cherokee_core. Pour cela, vous pouvez utiliser un sémaphore pour contrôler l'accès aux threads. Le sémaphore peut être initialisé avec le nombre maximum de threads autorisés à s'exécuter simultanément.

Lorsqu'un nouveau thread doit être créé pour gérer une demande de client, le sémaphore doit être acquis avant la création du thread. Si tous les threads sont en cours d'exécution, l'acquisition du sémaphore mettra en attente le thread jusqu'à ce qu'un autre thread se termine et libère le sémaphore.

Une fois le thread terminé, il doit libérer le sémaphore pour permettre à un autre thread de s'exécuter. Vous pouvez utiliser la fonction sem_init() pour initialiser le sémaphore, sem_wait() pour l'acquérir et sem_post() pour le libérer.

---

Un sémaphore pourrait être utilisé pour limiter le nombre de threads utilisés. Un sémaphore est un outil de synchronisation qui permet de limiter l'accès à une ressource partagée à un certain nombre de threads.

Dans le cas présent, on pourrait utiliser un sémaphore pour limiter le nombre de threads à la valeur maximale autorisée. Avant de créer un nouveau thread, on devrait alors attendre que le sémaphore soit disponible (ce qui signifie qu'un des threads existants doit être terminé), et après la fin d'un thread, on devrait libérer le sémaphore pour permettre à un autre thread de s'exécuter.

Cela peut se faire en utilisant les fonctions sem_wait() et sem_post() de la bibliothèque pthread. Par exemple, avant de créer un nouveau thread, on pourrait faire :

scss
Copy code
sem_wait(&semaphore); // attendre que le sémaphore soit disponible
pthread_create(&thread_id, NULL, thread_function, NULL); // créer un nouveau thread
Et à la fin du thread, on pourrait faire :

scss
Copy code
// code exécuté par le thread
sem_post(&semaphore); // libérer le sémaphore
pthread_exit(NULL); // terminer le thread
Cela garantirait que le nombre de threads ne dépasse jamais la valeur maximale autorisée, en attendant si nécessaire que d'autres threads se terminent avant de permettre à de nouveaux threads de s'exécuter.