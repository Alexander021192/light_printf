/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/08 18:08:23 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
#include "./includes/cub.h"
#include "./includes/libft.h"

#include <stdio.h>

void	my_mlx_pixel_put(t_win *win, int x, int y, int color)
{
    char    *dst;

    dst = win->addr + (y * win->line_lenght + x * (win->bpp / 8));
    *(unsigned int*)dst = color;
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
			my_mlx_pixel_put(win, point.x++, point.y, color);
		point.x -=SCALE;
		point.y++;
	}
}

// int		ft_draw_ray(t_all *all)
// {
// 	char **map = all->map;
// 	t_point		start;
// 	int 		x;
// 	int 		y;
// 	float		alpha;

// 	alpha  = -60;
// 	while (alpha < 60)
// 	{
// 		alpha = alpha + 0.001;
// 		x = 0;
// 		y = 0;
// 		start.x = all->plr->x * SCALE + 8;
// 		start.y = all->plr->y * SCALE + 8;
// 		//y = -tanh(45)*x;
// 		//printf("{%f} - x; {%f} - y", (x++ + start.x), (y + start.y));
// 		int i = 0;
// 		while(i++ < 100)
// 		{
// 			alpha = alpha + 0.1;
// 			mlx_pixel_put(all->win->mlx, all->win->win, (x++ + start.x), (tan((alpha*3.14)/180) * x + start.y), 0x1327ff);
// 			//ft_draw_my_pixel(all->win, point, 0xffffff);
// 			//printf("{%f}\n", tanh(alpha));
// 		}
// 	}
// 	return(0);
// }

int		ft_draw_player(t_all *all, t_point point, int color)
{
	float R = 0.;
	t_point start;
	start.x = (point.x * SCALE + 8);
	start.y = (point.y * SCALE + 8);
	int x;
	int	y;
	printf("\n{%f} {%f} init \n", start.x, start.y);
	while(R <= 10.)
	{
		x = start.x - R;
		y = 0;
		while (x <= (start.x + R))
		{
			y = sqrt(R*R - ((x - start.x)*(x - start.x))) + start.y;
			my_mlx_pixel_put(all->win, x, y, color);
			y = -sqrt(R*R - ((x - start.x)*(x - start.x))) + start.y;
			my_mlx_pixel_put(all->win, x, y, color);
			x++;
		}
		R += 0.08;
	}
	return(0);
}

int		ft_init_player(t_all *all, t_plr *plr)
{
	t_point		point;
	
	point.x = plr->x;
	point.y = plr->y;
	ft_bzero(&plr->x_move, sizeof(t_point));
	ft_bzero(&plr->y_move, sizeof(t_point));
	ft_bzero(&plr->rotate, sizeof(t_point));
	
	ft_draw_player(all, point, 0x99ff99);
	//ft_draw_ray(all);
	return(0);
}

int		player_move(t_all *all, int flag)
{
	t_point		point;
	
	point.x = all->plr->x;
	point.y = all->plr->y;
	ft_draw_player(all, point, 0x0);
	puts("+++++++++++++++++++");
	printf("BEFORE {%f}{%f}\n", all->plr->y,  all->plr->x);
	puts("+++++++++++++++++++");
	ft_draw_player(all, point, 0xff0000);
	if((all->plr->y_move.x || all->plr->y_move.y))
		point.y += 0.001 * flag;
	if((all->plr->x_move.x || all->plr->x_move.y)) 
		point.x += 0.001 * flag;
	all->plr->y = point.y;
	all->plr->x = point.x;
	puts("+++++++++++++++++++");
	printf("after {%f}{%f}\n", all->plr->y,  all->plr->x);
	puts("+++++++++++++++++++");
	//ft_draw_my_pixel(all->win, point, 0x24ff81);
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
				// придумать что то насчет направления! как и куда его записать!
				//можно сразу записать как тангенс угла такого тою
			}
			point.x++;
		}
		point.y++;
	}
	ft_init_player(all, all->plr);
	printf("\n{%f} {%f} init \n", all->plr->y_move.x, all->plr->y_move.y);
	return (0);
}

int
	key_press(int keycode, t_all *all)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		all->plr->y_move.x = 1;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		all->plr->y_move.y = 1;
	if (keycode == KEY_D)
		all->plr->x_move.y = 1;
	else if (keycode == KEY_A)
		all->plr->x_move.x = 1;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		all->plr->rotate.x = 1;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		all->plr->rotate.y = 1;
	return (0);
}

int		key_release(int keycode, t_all *all)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		all->plr->y_move.x = 0;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		all->plr->y_move.y = 0;
	if (keycode == KEY_D)
		all->plr->x_move.y = 0;
	else if (keycode == KEY_A)
		all->plr->x_move.x = 0;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		all->plr->rotate.x = 0;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		all->plr->rotate.y = 0;
	else if (keycode == KEY_ESC)
		return (mlx_destroy_window(all->win->mlx, all->win->win));
	return (0);
}

int		main_loop(t_all *all)
{
	if (all->plr->y_move.x || all->plr->y_move.y)
		player_move(all, (all->plr->y_move.x) ? -1 : 1);
	if (all->plr->x_move.x || all->plr->x_move.y)
		player_move(all, (all->plr->x_move.x) ? -1 : 1);
	// if (all->plr->rotate.x || all->plr->rotate.y)
	// 	update = rotate_camera(game, (game->rotate.x) ? 0 : 1);
	return(0);
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
	win.img = mlx_new_image(win.mlx, 640, 480);
	win.addr = mlx_get_data_addr(win.img, &win.bpp, &win.line_lenght, &win.en);
	all.plr = &plr;
	all.win = &win;
	draw_map(&all, 0);
	mlx_put_image_to_window(win.mlx, win.win, win.img, 50, 50);
	mlx_hook(win.win, 2, 1L<<0, key_press, &all);
	mlx_hook(win.win, 3, 1L<<1, key_release, &all);
	mlx_loop_hook(win.mlx, &main_loop, &all);
	mlx_loop(win.mlx);
	return (0);
}



