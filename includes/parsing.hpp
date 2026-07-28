/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/28 11:14:53 by ymoumene          #+#    #+#             */
/*   Updated: 2026/07/28 11:20:48 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include "webserv.hpp"

enum STATE {
	SERVER_SECTION,
	LOCATION_SECTION,
	MAIN_SECTION
};

class Request
{
	// un etat is_complete pour savoir si la requete est complete ou pas

	private:
		std::string							_start_line;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		long								_content_length;

	public:
		Request(void);
		Request(const Request &src);
		Request &operator=(const Request &src);
		~Request(void);
};


class Config
{
	private:
		STATE								_state;
		unsigned int 

	public:
		Config(void);
		Config(const Config &src);
		Config	&operator=(const Config &src);
		~Config(void);
		void	setState(STATE state);
};
	
std::vector<std::string>	*lexe_config(std::string filename);
void						lexer(std::string filename);
void						fsm(Config *config);

#endif