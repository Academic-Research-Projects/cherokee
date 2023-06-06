# Sockets

Un socket est un mécanisme qui permet à un programme de se connecter à unV réseau, d’envoyer et de recevoir des messages.

Les sockets permettent la communication entre 2 processus différents sur la même machine, ou entre deux machines.

Sous Unix, chaque action d'entrée et de sortie est réalisée en lisant ou en écrivant dans ce qu'on appelle un file descriptor (FD).
Un FD est un nombre entier unique au sein d'un même processus, associé à un fichier actuellement ouvert qui peut être lié à un terminal, une connexion réseau ou un fichier texte par exemple.

Les sockets sont des types de FD.


# Multiplexing

Le plus simple pour créer un serveur web capable de prendre en charge un maximum de requêtes entrantes et d'y répondre, est de créer un nouveau thread pour chaque connexion client.
Or, chaque basculement entre 2 threads est une perte de performance.

Une meilleure solution est de contrôler l'ensemble des lectures et écritures en un seul thread principal, au lieu de se reposer sur la machine : c'est le multiplexing.
Cela implique un développement plus poussé, car il faut pouvoir manipuler l'ensemble des connexions clientes du début à la fin.

Le principe quel que soit l'implémentation est le suivant : on observe les sockets (qui sont des FD) clients, et l'on intéragie avec eux seulement quand ils sont prêts en lecture ou écriture.
De cette manière chaque requête est traitée une par une, et on s'attend à ce que le programme traite en premier les requêtes de petite taille.


# epoll()

Nous avons fait le choix d'implémenter le mécanisme le plus récent pour du multiplexing : la méthode epoll().

Cette fonction surveille les FD qui ont été au préalable ajoutés dans ce qu'on appelle l'instance epoll.
Elle attend jusqu'au moment ou quelque chose se passe sur un des FD, et renvoie l' (ou les) évènement qui a été détecté (demande de connexion d'un nouveau client, un client existant a fini d'envoyer sa requête...)
C'est à ce moment que l'on doit manuellement déclencher l'action appropriée pour chaque évènement, et c'est pour cela que l'on doit constamment keep track de l'état des sockets connectés.


# Explication dans les grandes lignes du code ???