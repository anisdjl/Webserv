# Tests du parseur de requetes HTTP

Ce dossier est independant du Makefile principal. Il ne modifie ni le code du
webserv, ni sa compilation normale. Il est prevu pour Fedora Linux et C++98.

## Installation sur Fedora

```sh
sudo dnf install gcc-c++ make file
cd parser_tests
make
```

Le parseur actuel du depot est encore en cours d'ecriture. Tant que ses
declarations et definitions ne correspondent pas, `make` est donc cense
signaler les erreurs de compilation du parseur.

## Tests automatiques

```sh
make test
make verbose
```

Les neuf tests couvrent :

- un GET simple ;
- une requete recue octet par octet ;
- un POST avec `Content-Length` ;
- la casse des noms de headers ;
- `Transfer-Encoding: chunked` ;
- un body incomplet ;
- l'absence de `Host` en HTTP/1.1 ;
- le conflit `Content-Length` / `Transfer-Encoding` ;
- deux requetes presentes dans le meme `recv()`.

`make verbose` affiche les champs parses, le body avec les caracteres speciaux
echappes et le nombre d'octets encore conserves dans le buffer.

## Tester une requete personnalisee

Depuis un fichier contenant exactement les octets recus :

```sh
make inspect FILE=ma_requete.raw
make inspect FILE=ma_requete.raw CHUNK=1
```

`CHUNK=1` simule un `recv()` qui ne fournit qu'un octet a la fois. Une autre
taille, par exemple `CHUNK=7`, permet de verifier que le resultat ne depend pas
du decoupage des paquets TCP.

Depuis l'entree standard :

```sh
printf 'GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n' | make inspect CHUNK=1
```

Pour une requete incomplete, le programme affiche l'etat `INCOMPLETE`, les
octets encore en buffer et termine avec un code d'erreur.

## Compiler le webserv hors reponses et CGI

```sh
make core-check
```

Ce target compile separement toutes les unites `.cpp` du projet sauf
`srcs/http/HttpResponse.cpp` et `srcs/sockets/Cgi.cpp`. Il ne fait volontairement
pas l'edition des liens : les autres classes possedent encore des references a
ces deux composants exclus. Ce controle permet tout de meme de trouver les
erreurs C++ dans le parsing, la configuration, les sockets et la boucle du
serveur.

## A propos du binaire `tester` fourni

Le fichier `../../tester` a la racine exterieure est un executable Linux x86-64.
Sur Fedora :

```sh
chmod +x ../../tester
```

Depuis ce dossier, on peut afficher ses informations avec :

```sh
make tester-info
```

Puis, uniquement lorsque le webserv complet repond deja dans un autre terminal :

```sh
make official-tester
```

Ce binaire teste des reponses HTTP et des CGI. Il n'est donc pas adapte pour
valider uniquement le parsing des requetes ; `request_parser_test` sert a cette
etape intermediaire.
