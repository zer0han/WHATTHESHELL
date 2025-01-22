/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expsn_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 12:37:36 by rdalal            #+#    #+#             */
/*   Updated: 2025/01/20 15:27:12 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* fiunction that expands the variable in $variable for
* works like this echo $USER gives rdalal, should work with env variables too for something like
** $PATH which comes out as /home/rdalal/bin:/home/rdalal/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
*** this is just an example
*/