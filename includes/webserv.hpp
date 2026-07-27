#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iomanip>
# include <iostream>
# include <cstring> // pour strerror
# include <netdb.h> // pour gai_strerror, getaddrinfo, freeaddrinfo, getsockname, getprootobyname
# include <unistd.h> // pour execve, pipe, dup et dup2 et fork, chdir, close, read, write, acces
# include <poll.h> // pour poll et epoll
# include <cerrno> // pour errno mais pas sur que ca soit dispo sur cpp98
# include <sys/socket.h> // socketpair, bind, listen, accept, connect, recv, send, getsockname, setsockopt
# include <arpa/inet.h> // pour htonl, htons, ntohl, ntohs
# include <sys/epoll.h> // pour epoll, epoll_create, epoll_ctl, epoll_wait
# include <dirent.h> // pour opendir, readdir, closedir
# include <fcntl.h> // pour fcntl
# include <sys/types.h>
# include <csignal> // pour signal
# include <sys/wait.h> // waitpid
# include <sys/stat.h> // pour stat

#endif