/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/11 16:15:32 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
#include "./includes/cub.h"
#include "./includes/libft.h"

#include <stdio.h>

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->ptr + (y * img->line_lenght + x * (img->bpp / 8));
	*(unsigned int*)dst = color;
}

void	ft_draw_my_pixel(t_image *img, t_point point, int color)
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
			my_mlx_pixel_put(img, point.x++, point.y, color);
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
void	ft_clear_img(t_image *img)
{
	int x = 0, y = 0;
	while(x < 640)
	{
		y = 0;
		while (y < 480)
			my_mlx_pixel_put(img, x, y++, 0xFF000000);
		x++;
	}
}

int		ft_draw_player(t_all *all, t_point point, int color)
{
	int x = 0, y = 0;
	double R = 0.;
	
	t_point start;

	start.x = (point.x * SCALE + 8);
	start.y = (point.y * SCALE + 8);
	
	ft_clear_img(&all->plr->plr_scr);
	
	printf("\n{%f} - x {%f} - y; and dir {%f} init position player \n", start.x, start.y, all->plr->dir);
	while(R <= 10.)
	{
		x = start.x - R;
		y = 0;
		while (x <= (start.x + R))
		{
			y = sqrt(R*R - ((x - start.x)*(x - start.x))) + start.y;
			my_mlx_pixel_put(&all->plr->plr_scr, x, y, color);
			y = -sqrt(R*R - ((x - start.x)*(x - start.x))) + start.y;
			my_mlx_pixel_put(&all->plr->plr_scr, x, y, color);
			x++;
		}
		R += 0.08;
	}
	mlx_put_image_to_window(all->win->win, all->win->win, all->plr->plr_scr.img, 75, 75);
	return(0);
}


int		ft_init_player(t_all *all, t_image *all_plr_scr)
{
	t_point		point;
	t_image		plr_scr;
	
	plr_scr.img = mlx_new_image(all->win->ptr, 640, 480);
	plr_scr.ptr = mlx_get_data_addr(plr_scr.img, &plr_scr.bpp, &plr_scr.line_lenght, &plr_scr.endian);
	all->plr->plr_scr = plr_scr;
	
	point.x = all->plr->pos.x;
	point.y = all->plr->pos.y;
	ft_bzero(&all->plr->x_move, sizeof(t_point));
	ft_bzero(&all->plr->y_move, sizeof(t_point));
	ft_bzero(&all->plr->rotate, sizeof(t_point));
	
	
	ft_draw_player(all, point, 0xf800);
	//ft_draw_ray(all);
	return(0);
}

int		player_move(t_all *all, int flag)
{
	t_point		point;
	
	point.x = all->plr->pos.x;
	point.y = all->plr->pos.y;
	// puts("+++++++++++++++++++");
	//printf("\n{%f} - x {%f} - y; and dir {%f} before position player \n", point.x, point.y, all->plr->dir);
	// puts("+++++++++++++++++++");
	//ft_draw_player(all, point, 0xff0000);
	if((all->plr->y_move.x || all->plr->y_move.y))
		point.y += 0.0001 * flag;
	if((all->plr->x_move.x || all->plr->x_move.y)) 
		point.x += 0.0001 * flag;
	if(point.x > 0 && point.y > 0 && point.x * SCALE < 640 && point.y * SCALE < 480)
	{
		all->plr->pos.y = point.y;
		all->plr->pos.x = point.x;
		ft_draw_player(all, point, 0xff2aff);
		// puts("+++++++++++++++++++");
		//printf("\n{%f} - x {%f} - y; and dir {%f} after position player \n", point.x, point.y, all->plr->dir);
		// puts("+++++++++++++++++++");
	}
	//ft_draw_my_pixel(all->win, point, 0x24ff81);
	return(0);
}

int		draw_map(t_all *all, t_image *img_map)
{
	t_point		point;
		
	img_map->img = mlx_new_image(all->win->ptr, 640, 480);
	img_map->ptr = mlx_get_data_addr(img_map->img, &img_map->bpp, &img_map->line_lenght, &img_map->endian);
	
	
	ft_bzero(&point, sizeof(t_point));
	
	while(all->map_arr[(int)point.y])
	{
		point.x = 0;
		while (all->map_arr[(int)point.y][(int)point.x])
		{
			if(all->map_arr[(int)point.y][(int)point.x] == '1')
				//mlx_pixel_put(win->mlx, win->win, point.x, point.y, 0xffffff);
				ft_draw_my_pixel(img_map, point, 0xffffff);
			else if(ft_strchr("SWNE", all->map_arr[(int)point.y][(int)point.x]))
			{
				all->plr->pos.x = point.x;
				all->plr->pos.y = point.y;
				if(all->map_arr[(int)point.y][(int)point.x] == 'E')
					all->plr->dir = 0;
				else if(all->map_arr[(int)point.y][(int)point.x] == 'S')
					all->plr->dir = 90;
				else if(all->map_arr[(int)point.y][(int)point.x] == 'W')
					all->plr->dir = 180;
				else if(all->map_arr[(int)point.y][(int)point.x] == 'N')
					all->plr->dir = 270;
			}
			point.x++;
		}
		point.y++;
	}
	mlx_put_image_to_window(all->win->ptr, all->win->win, img_map->img, 75, 75);
	return (0);
}

int		key_press(int keycode, t_all *all)
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
		return (mlx_destroy_window(all->win->ptr, all->win->win));
	return (0);
}


int		main_loop(t_all *all)
{
	if (all->plr->y_move.x || all->plr->y_move.y)
		player_move(all, (all->plr->y_move.x) ? -1 : 1);
	if (all->plr->x_move.x || all->plr->x_move.y)
		printf("move right/left\n");
		//player_move(all, (all->plr->x_move.x) ? -1 : 1);
	 if (all->plr->rotate.x || all->plr->rotate.y)
	 	printf("move rotate\n");
	// 	update = rotate_camera(game, (game->rotate.x) ? 0 : 1);
	return(0);
}

void	set_pos(t_point *pos, double x, double y)
{
	pos->x = x;
	pos->y = y;
}

void	init_start_plr(t_plr *plr)
{
	t_image		plr_scr;
	//init config - texture, resolutin, size;
	plr_scr.img = NULL;
	plr_scr.ptr = NULL;

	plr->plr_scr = plr_scr;
}

int		main(void)
{
	t_win	win;
	t_plr	plr;
	t_all	all;

	all.map_arr = ft_read_map();

	// int	i = 0;
	// while (all.map_arr[i])
	// {
	// 	ft_putendl_fd(all.map_arr[i], 1);
	// 	i++;
	// }
	
	//ft_init_player(all.map, &plr);
	
	init_start_plr(&plr);
	win.ptr = mlx_init();
	win.win = mlx_new_window(win.ptr, 640, 480, "cub2d");
	all.plr = &plr;
	all.win = &win;
	draw_map(&all, &all.img_map);
	ft_init_player(&all, &plr.plr_scr);

	// mlx_hook(win.win, 2, 1L<<0, key_press, &all);
	// mlx_hook(win.win, 3, 1L<<1, key_release, &all);
	// mlx_loop_hook(win.ptr, &main_loop, &all);
	mlx_loop(win.ptr);
	return (0);
}



