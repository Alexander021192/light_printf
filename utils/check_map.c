/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandr <alexandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 13:42:58 by alexandr          #+#    #+#             */
/*   Updated: 2021/03/17 13:46:50 by alexandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int		ft_find_plr_pos(char **map, t_point *plr_pos)
{
	int		count_pos;
	t_point	pos;

	count_pos = 0;
	pos.x = 0;
	pos.y = 0;
	while (map[(int)pos.y])
	{
		while (MAP(pos.y, pos.x))
		{
			if (ft_strchr("NWSE", MAP(pos.y, pos.x)))
			{
				plr_pos->x = pos.x;
				plr_pos->y = pos.y;
				count_pos++;
			}
			pos.x++;
		}
		pos.x = 0;
		pos.y++;
	}
	return (count_pos);
}

int		map_step(char **map, t_point *pos, int pres_step)
{
	while (1)
	{
		if (pres_step == RIGHT && (int)pos->y &&
		MAP(pos->y - 1, pos->x) == '1' && pos->y--)
			return (UP);
		else if (pres_step == UP && (int)pos->x &&
		MAP(pos->y, pos->x - 1) == '1' && pos->x--)
			return (LEFT);
		else if (pres_step == LEFT && map[(int)pos->y + 1] &&
		MAP(pos->y + 1, pos->x) == '1' && ++pos->y)
			return (DOWN);
		else if (pres_step == DOWN &&
		MAP(pos->y, pos->x + 1) == '1' && ++pos->x)
			return (RIGHT);
		pres_step += (pres_step == 44) ? -33 : 11;
	}
}

int		init_start_pos(char **map, t_point *pos, t_point *start_pos)
{
	start_pos->x = pos->x;
	while ((int)pos->y != 0 && MAP(pos->y, pos->x) != ' ')
		pos->y--;
	if ((int)pos->y == 0 && MAP(pos->y, pos->x) == '1')
	{
		start_pos->y = pos->y;
		return (1);
	}
	else if (MAP(pos->y, pos->x) == ' ' && MAP(pos->y + 1, pos->x) == '1')
	{
		start_pos->y = ++pos->y;
		return (1);
	}
	else
		return (0);
}

int		check_map(char **map)
{
	t_point	pos;
	t_point	start_pos;
	int		pres_step;

	pres_step = RIGHT;
	if (ft_find_plr_pos(map, &pos) != 1)
		return (printf("error with position player\n"));
	if (init_start_pos(map, &pos, &start_pos))
	{
		pres_step = map_step(map, &pos, pres_step);
		while (start_pos.x != pos.x || start_pos.y != pos.y)
			pres_step = map_step(map, &pos, pres_step);
	}
	if (pres_step == LEFT || pres_step == UP ||
	!init_start_pos(map, &pos, &start_pos))
		return (printf("error! not close map"));
	return (0);
}
