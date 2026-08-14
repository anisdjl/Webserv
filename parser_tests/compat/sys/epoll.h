#ifndef PARSER_TESTS_COMPAT_SYS_EPOLL_H
#define PARSER_TESTS_COMPAT_SYS_EPOLL_H

/*
 * HttpRequest.hpp inclut actuellement Webserv.hpp, qui inclut sys/epoll.h.
 * Le parseur de requete n'utilise pourtant aucune API epoll. Ce fichier vide
 * permet donc de compiler uniquement les tests du parseur sur macOS aussi.
 * Il n'est jamais utilise par le Makefile principal du webserv.
 */

#endif
