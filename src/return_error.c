/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:13:58 by discallow         #+#    #+#             */
/*   Updated: 2024/10/14 20:45:23 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	invalid_args(void)
{
	printf(RED"Invalid input."RESET"\n");
	printf(CYAN"Valid input example: ./philo 5 800 200 200 [5]" RESET"\n");
	return (1);
}
long	invalid_input(const char *s)
{
	printf(RED"\"%s\": Invalid input."RESET"\n", s);
	return (LONG_MIN);
}
