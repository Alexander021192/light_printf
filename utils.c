/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:11:29 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/24 14:21:51 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub.h"
#include "./includes/libft.h"
#include "./includes/mlx.h"
#include "./includes/utils.h"

#include <stdio.h>

int		rgb_to_hex(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

int		key_press(int keycode, t_all *all)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		all->y_move.x = 1;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		all->y_move.y = 1;
	if (keycode == KEY_D)
		all->x_move.y = 1;
	else if (keycode == KEY_A)
		all->x_move.x = 1;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		all->rotate.x = 1;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		all->rotate.y = 1;
	return (0);
}

int		key_release(int keycode, t_all *all)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		all->y_move.x = 0;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		all->y_move.y = 0;
	if (keycode == KEY_D)
		all->x_move.y = 0;
	else if (keycode == KEY_A)
		all->x_move.x = 0;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		all->rotate.x = 0;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		all->rotate.y = 0;
	else if (keycode == KEY_ESC)
	{
		printf("window closed\n");
		return (mlx_destroy_window(all->win.ptr, all->win.win));
	}
	return (0);
}

int		y_update_pos(t_all *all, int flag)
{
	t_point	old_pos;
	t_point new_pos;
	t_plr	*plr;
	double	dir;
	
	plr	= &all->plr;
	old_pos.x = plr->pos.x;
	old_pos.y = plr->pos.y;
	dir = plr->dir * (M_PI_2 / 180) + M_PI_2;
	printf("{%f} - x; {%f} - y; {%d} - dir: pos\n", plr->pos.x, plr->pos.y, plr->dir);
	
	new_pos.y = old_pos.y + flag * SPEED;
	//plr->pos.y = new_pos.y;
	plr->pos.x = - (new_pos.y - old_pos.y) * sin(dir) + old_pos.x;
	plr->pos.y = (new_pos.y - old_pos.y) * cos(dir) + old_pos.y;
	if(all->map_arr[(int)plr->pos.y/SCALE][(int)plr->pos.x/SCALE] == '1')
	{
		plr->pos.x = old_pos.x;
		plr->pos.y = old_pos.y;
		return (0);
	}
	return(1);	
}

int		x_update_pos(t_all *all, int flag)
{
	t_point	old_pos;
	t_point new_pos;
	t_plr	*plr;
	double	dir;
	
	plr	= &all->plr;
	old_pos.x = plr->pos.x;
	old_pos.y = plr->pos.y;
	dir = plr->dir * (M_PI_2 / 180) + M_PI_2;

	
	new_pos.x = old_pos.x + flag * SPEED;
	plr->pos.x = (new_pos.x - old_pos.x) * cos(dir) + old_pos.x;
	plr->pos.y = (new_pos.x - old_pos.x) * sin(dir) + old_pos.y;
	
	if(all->map_arr[(int)plr->pos.y/SCALE][(int)plr->pos.x/SCALE] == '1')
	{
		plr->pos.x = old_pos.x;
		plr->pos.y = old_pos.y;
	}
	
	return(1);	
}

int		rotate_camera(t_all *all, int flag)
{
	all->plr.dir = all->plr.dir % 720 + flag * 2;
	return (1);
}

int		main_loop(t_all *all)
{
	//printf("{%f}{%f}{%d} in in update x y and dir pos\n", all->plr.pos.x, all->plr.pos.y, all->plr.dir);

	int	update = 0;
	if (all->y_move.x || all->y_move.y)
		//printf("move up/down\n");
	 	update = y_update_pos(all, (all->y_move.x) ? -1 : 1);
	if (all->x_move.x || all->x_move.y)
		//printf("move right/left\n");
	 	update = x_update_pos(all, (all->x_move.x) ? -1 : 1);
	 if (all->rotate.x || all->rotate.y)
	 	//printf("move rotate\n");
		update = rotate_camera(all, (all->rotate.x) ? -1 : 1);
	if(update == 1)
		update_screen(all);
	return(0);
}

void	set_pos(t_point *pos, double x, double y)
{
	pos->x = x;
	pos->y = y;
}

void	my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int*)dst = color;
}

void	ft_draw_my_pixel(t_all *all, t_point point, t_point pxl_size, int color)
{
	t_point	end;
	
	point.x *= pxl_size.x;
	point.y *= pxl_size.y;
	end.x = point.x + pxl_size.x;
	end.y = point.y + pxl_size.y;

	while (point.y < end.y)
	{
		while (point.x < end.x)
			my_mlx_pixel_put(&all->screen, point.x++, point.y, color);
		point.x -= pxl_size.x;
		point.y++;
	}
}

int		ft_init_player(t_all *all)
{
	t_plr	plr;
	t_point	point;
	
	ft_bzero(&plr.pos, sizeof(t_point));
	ft_bzero(&point, sizeof(t_point));
	ft_bzero(&all->x_move, sizeof(t_point));
	ft_bzero(&all->y_move, sizeof(t_point));
	ft_bzero(&all->rotate, sizeof(t_point));
	
	while(all->map_arr[(int)point.y])
	{
		point.x = 0;
		while (all->map_arr[(int)point.y][(int)point.x])
		{
			if(ft_strchr("SWNE", all->map_arr[(int)point.y][(int)point.x]))
			{
				plr.pos.x = point.x * SCALE + SCALE/2; //change on pxl_size
				plr.pos.y = point.y * SCALE + SCALE/2;
				if(all->map_arr[(int)point.y][(int)point.x] == 'E')
					plr.dir = 0;
				else if(all->map_arr[(int)point.y][(int)point.x] == 'S')
					plr.dir = 180;
				else if(all->map_arr[(int)point.y][(int)point.x] == 'W')
					plr.dir = 360;
				else if (all->map_arr[(int)point.y][(int)point.x] == 'N')
					plr.dir = 540;
				all->plr = plr;
				return (0);
			}
			point.x++;
		}
		point.y++;
	}
	return (1);
}

int		ft_draw_map(t_all *all)
{

	t_point		point;
	t_point		pxl_size;

	pxl_size.x = 16;
	pxl_size.y = 16;

	ft_bzero(&point, sizeof(t_point));
	while(all->map_arr[(int)point.y])
	{
		point.x = 0;
		while (all->map_arr[(int)point.y][(int)point.x])
		{
			if(all->map_arr[(int)point.y][(int)point.x] == '1')
				ft_draw_my_pixel(all, point, pxl_size, 0xffffff);
			point.x++;
		}
		point.y++;
	}
	return (0);
}

int		ft_draw_player(t_all *all)
{
	t_plr	*plr;
	t_point	pos;
	double	angle;
	double	len_ray;

	plr = &all->plr;
	angle = plr->dir - 60;
	while(angle < plr->dir + 60)
	{
		len_ray = 0;
		pos.x = plr->pos.x;
		pos.y = plr->pos.y;
		while(all->map_arr[(int)pos.y / SCALE][(int)pos.x / SCALE] != '1')
		{
			pos.x = plr->pos.x + len_ray * cos(angle * M_PI_2/180);
			pos.y = plr->pos.y + len_ray * sin(angle * M_PI_2/180);
			my_mlx_pixel_put(&all->screen, pos.x, pos.y, 0x80ff00);
			len_ray += 1;
		}
		angle += 0.5;
	}
	return(0);
}

// void	update_screen(t_all *all)
// {
// 	all->screen->img = mlx_new_image(all->win->ptr, 640, 480);
// 	all->screen->addr = mlx_get_data_addr(all->screen->img, &all->screen->bpp,
// 	&all->screen->line_len, &all->screen->endian);
	
// 	ft_draw_map(all);
	
	
	
// 	ft_draw_player(all);
	
// 	mlx_put_image_to_window(all->win->ptr, all->win->win, all->screen->img, 0, 0);
// 	mlx_destroy_image(all->win->ptr, all->screen->img);
// }