/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymoumene <ymoumene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 15:21:35 by ymoumene          #+#    #+#             */
/*   Updated: 2026/08/07 15:22:55 by ymoumene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket/Cgi.hpp"





int CGI::getParentIndex() const
{
	return _parent_index;
}

void CGI::setParentIndex(int index)
{
	_parent_index = index;
}
