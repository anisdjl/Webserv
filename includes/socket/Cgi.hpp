/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjelili <adjelili@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 20:02:41 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/24 17:44:26 by adjelili         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP


#include "Socket.hpp"

class Cgi : public Socket
{
    private :
        int 	   		_parent_index;
		int				_pipe_in;
		int				_pipe_out;
		int				_child_fd;
		int				_epollfd;
		time_t 			_timestamp;
		std::string		_reqPath;
		std::string		_bodyToWrite;
		ssize_t			_bodyWritten;

    public:
		void	setParentIndex(int &index) { this->_parent_index = index; };
		void	setPipeIn(int &pipe) { _pipe_in = pipe; };
		void	setPipeOut(int &pipe) { _pipe_out = pipe; };
		void	setFd(int &fd) { _child_fd = fd; };
		void	setEpoll(const int &epoll) { _epollfd = epoll; };
		void	setBeginExec() { _timestamp = std::time(NULL); };
		void	setReqPath(std::string path) { _reqPath = path; }; 
		time_t	getTime(void) { return (_timestamp); };
		int		getParentIndex() const { return (this->_parent_index); };
		int		getPipeIn(void) { return (_pipe_in); };
		int		getPipeOut(void) { return (_pipe_out); };
		int		getEpoll(void) { return (_epollfd); };
		int		getChildFd(void) { return (_child_fd); };
		ssize_t	&getBodyWritten(void) { return (_bodyWritten); };
		
		void	addWrittenBytes(ssize_t &size) { _bodyWritten += size; };
		std::string	getReqPath(void) { return _reqPath; };
		
		Cgi() : Socket(), _parent_index(-1), _bodyWritten(0) { this->_type = CGI; };
		Cgi(int fd, int server_index, int parent_index) : Socket(fd, server_index, CGI), _parent_index(parent_index) {	};
		Cgi(const Cgi& src) : Socket(src), _parent_index(src._parent_index) { };
        ~Cgi() {};
        
		Cgi& operator=(const Cgi& src)
		{
			if (this != &src)
			{
				this->_fd = src._fd;
				this->_server_index = src._server_index;
				this->_type = src._type;
				this->_parent_index = src._parent_index;
			}
			return (*this);
		};
};

bool		ft_cgi_in(std::map<int, Socket*> &map_socket, Cgi &target, Config *config);
void		ft_cgi_hup(std::map<int, Socket*> &map_socket, Cgi &target, Config *config);
bool 		ft_cgi_out(std::map<int, Socket*> &map_socket, Cgi &target, Config *config);
void		ft_cgi_close(Cgi &target, std::map<int, Socket *> &map_socket);
void		checkContentTypeScript(Connection &client);


#endif