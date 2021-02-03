/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/03 17:32:02 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
#include "./includes/cub.h"
#include "./includes/libft.h"

#include <stdio.h>

int		rgb_to_hex(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

void	ft_draw_my_pixel(t_win *win, t_point point, int color)
{
	t_point	end;
	
	//point.x += 50;
	//point.y += 50;
	
	end.x = (point.x + 1) * SCALE;
	end.y = (point.y + 1) * SCALE;
	point.x *= SCALE;
	point.y *=SCALE;

	while (point.y < end.y)
	{
		while (point.x < end.x)
			mlx_pixel_put(win->mlx, win->win, point.x++, point.y, color);
		point.x -=SCALE;
		point.y++;
	}
}

int		ft_init_player(t_all *all, t_plr *plr)
{
	t_point		point;
	
	point.x = plr->x;
	point.y = plr->y;
	ft_draw_my_pixel(all->win, point, 0x24ff81);
	return(0);
}

int		ft_draw_player(t_all *all, t_plr *plr)
{
	t_point		point;
	
	point.x = plr->x;
	point.y = plr->y;
	ft_draw_my_pixel(all->win, point, 0x0);
	point.y += 0.2;
	all->plr->y = point.y;
	ft_draw_my_pixel(all->win, point, 0x24ff81);
	return(0);
}

int		draw_map(t_all *all, int a)
{
	t_point		point;
	t_win		*win = all->win;

	//win->img = mlx_new_image(win->mlx, 640, 480);
	//win->addr = mlx_get_data_addr(win->img, &win->bpp, &win->line_l, &win->en);
	ft_bzero(&point, sizeof(t_point));
	
	while(all->map[(int)point.y])
	{
		point.x = 0;
		while (all->map[(int)point.y][(int)point.x])
		{
			if(all->map[(int)point.y][(int)point.x] == '1')
				//mlx_pixel_put(win->mlx, win->win, point.x, point.y, 0xffffff);
				ft_draw_my_pixel(all->win, point, 0xffffff);
			else if(ft_strchr("SWNE", all->map[(int)point.y][(int)point.x]))
			{
				all->plr->x = point.x;
				all->plr->y = point.y;
			}
			point.x++;
		}
		point.y++;
	}
	ft_init_player(all, all->plr);
	return (0);
}

int		key_press(int keycode, t_all *all)
{
	if (keycode == 53)
		mlx_destroy_window(all->win->mlx, all->win->win);
	if(keycode == 126)
	{
		//printf("{%d}, {%d}\n", all->plr->x, all->plr->y);
		ft_draw_player(all, all->plr);
		//printf("{%d}, {%d}\n", all->plr->x, all->plr->y);
	}
	else //можем печатать номера печаемых кнопок
		printf(" you press button number {%d}\n", keycode);
	return (0);
}

int		main(void)
{
	t_win	win;
	t_plr	plr;
	t_all	all;

	all.map = ft_read_map();
	int	i = 0;
	while (all.map[i])
	{
		ft_putendl_fd(all.map[i], 1);
		i++;
	}
	//ft_init_player(all.map, &plr);
	win.mlx = mlx_init();
	win.win = mlx_new_window(win.mlx, 640, 480, "cub2d");
	//win.img = mlx_get_data_addr(win.img, &win.bpp, &win.line_l, &win.en);
	all.plr = &plr;
	all.win = &win;
	draw_map(&all, 0);
	mlx_hook(win.win, 2, 1L<<0, key_press, &all);
	mlx_loop(win.mlx);
	return (0);
}



