/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:11:29 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/27 16:40:39 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub.h"
#include "./includes/libft.h"
#include "./includes/mlx.h"
#include "./includes/utils.h"

#include <stdio.h>

int		get_tex_color(t_tex *tex, int x, int y)
{
	return (*(int*)(tex->ptr + (tex->size_line * y) + (tex->bpp / 8 * x)));
}

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
	//printf("{%f} - x; {%f} - y; {%d} - dir: pos\n", plr->pos.x, plr->pos.y, plr->dir);
	
	new_pos.y = old_pos.y + flag * SPEED;
	//plr->pos.y = new_pos.y;
	plr->pos.x = - (new_pos.y - old_pos.y) * sin(dir) + old_pos.x;
	plr->pos.y = (new_pos.y - old_pos.y) * cos(dir) + old_pos.y;
	if(all->map_arr[(int)plr->pos.y/(int)all->map_size.y][(int)plr->pos.x/(int)all->map_size.x] == '1')
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
	
	if(all->map_arr[(int)plr->pos.y/(int)all->map_size.y][(int)plr->pos.x/(int)all->map_size.x] == '1')
	{
		plr->pos.x = old_pos.x;
		plr->pos.y = old_pos.y;
	}
	
	return(1);	
}

int		rotate_camera(t_all *all, int flag)
{
	all->plr.dir = (all->plr.dir + 720) % 720 + flag * 2;
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
				plr.pos.x = point.x * all->map_size.x + all->map_size.x/2; //change on pxl_size
				plr.pos.y = point.y * all->map_size.y + all->map_size.y/2;
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

int		load_tex(t_all *all, t_tex *tex, char *path)
{
	if (path)
	{
		tex->path = path;
		if ((tex->tex = mlx_xpm_file_to_image(all->win.ptr,
			path, &tex->width, &tex->height)))
			tex->ptr = mlx_get_data_addr(tex->tex,
				&tex->bpp, &tex->size_line, &tex->endian);
		else
			return (0);
	}
	return (1);
}

int		ft_draw_map(t_all *all)
{

	t_point		point;

	ft_bzero(&point, sizeof(t_point));
	while(all->map_arr[(int)point.y])
	{
		point.x = 0;
		while (all->map_arr[(int)point.y][(int)point.x])
		{
			if(all->map_arr[(int)point.y][(int)point.x] == '1')
				ft_draw_my_pixel(all, point, all->map_size, 0xffffff);
			point.x++;
		}
		point.y++;
	}
	return (0);
}

int		ft_draw_back(t_all *all)
{
	t_point	sky_floor;

	ft_bzero(&sky_floor, sizeof(t_point));
	
	while(sky_floor.y < all->win_height / 2)
	{
		sky_floor.x = 0;
		while(sky_floor.x < all->win_width)
			//printf("{%f sky x}{%f sky y}\n", sky_floor.x++, sky_floor.y);
			my_mlx_pixel_put(&all->screen, sky_floor.x++, sky_floor.y, 0x0aa0c9);
		sky_floor.y++;
	}
	//--------------------------------------------//

	//----------------func draw floor-------------//
	while(sky_floor.y < all->win_height)
	{
		sky_floor.x = 0;
		while(sky_floor.x < all->win_width)
			//printf("{%f sky x}{%f sky y}\n", sky_floor.x++, sky_floor.y);
			my_mlx_pixel_put(&all->screen, sky_floor.x++, sky_floor.y, 0x693800);
		sky_floor.y++;
	}
	return (0);
}

int		ft_draw_wall(t_all *all, t_ray ray) 
{
	double		column_height;
	t_point	pos;

	ray.dir = ((int)ray.dir + 720) % 720;
			
	ray.len_ray = (ray.len_ray * cos((ray.dir - all->plr.dir) * M_PI_2/180)) / all->map_size.y;
	column_height = (all->win_height/ray.len_ray);



	int		x_texcoord;
	int		y_texcoord;

	
	//----------func get x_texcoord ----------//
	double	hitx, hity;
	hity = ray.pos.y/all->map_size.y - (int)(ray.pos.y/all->map_size.y + 0.5);
	hitx = ray.pos.x/all->map_size.x - (int)(ray.pos.x/all->map_size.x + 0.5); // как сделать, чтобы не видеть самый самый край стены

	//-----------//func get number tex-------------
	int num_tex = 0;
	
	if(ABS(hity) > ABS(hitx))
	{
		if(ray.dir > 180 && ray.dir < 540)
			num_tex = 3;
		else
			num_tex = 1;
	}
	else if(ABS(hity) < ABS(hitx))
	{
		//printf("{%f ray dir}\n", ray.dir);
		if(ray.dir > 0 && ray.dir < 360)
			num_tex = 2;
		else
			num_tex = 0;
	}

	x_texcoord = MAX(ABS(hity), ABS(hitx)) * all->tex->width;
	
	// -----------------------------//
	y_texcoord = 0;
	pos.x = ray.num_ray;
	pos.y = (all->win_height/2 - column_height/2);
	//printf("{%d} texcoord\n", x_texcoord);
	//printf("{%d}{%d} tex config\n", all->tex[0].width, all->tex[0].height);

	int i = (column_height > all->win_height) ? (int)(-pos.y) : 0;
	pos.y = (pos.y < 0) ? 0 : pos.y;
	//printf("{%d column %f pos.y {%d - i} \n", column_height, pos.y, i);
	 
	while(i < column_height)
	{
		y_texcoord = (i++ * 64) / column_height;
		if(pos.y > 0 && pos.y < all->win_height)  // есть момент с тем, лишниими циклами при высоких стенах
			my_mlx_pixel_put(&all->screen, pos.x, pos.y, 
				get_tex_color(&all->tex[num_tex], x_texcoord, y_texcoord));
		else if(pos.y > all->win_height)
			break;
	
		pos.y++;

	}

	return(0);
}

int		ft_draw_sprite(t_all *all, t_sprite sprite)
{
	double	sprite_height;
	t_point	pos;

	//ray.dir = ((int)ray.dir + 720) % 720;
			
	sprite.distance = (sprite.distance) / all->map_size.y; // * cos((ray.dir - all->plr.dir) * M_PI_2/180))
	sprite_height = (all->win_height/sprite.distance);



	int		x_texcoord;
	int		y_texcoord;
	int		color;

	
	//----------func get x_texcoord ----------//
	// double	hitx, hity;
	// hity = sprite.pos.y/all->map_size.y - (int)(sprite.pos.y/all->map_size.y + 0.5);
	// hitx = sprite.pos.x/all->map_size.x - (int)(sprite.pos.x/all->map_size.x + 0.5); // как сделать, чтобы не видеть самый самый край стены

	// x_texcoord = ABS(hitx) * all->tex->width;
	x_texcoord = 0;
	// -----------------------------//
	y_texcoord = 0;
	pos.x = sprite.num_ray;
	pos.y = (all->win_height/2 - sprite_height/2);
	//printf("{%d} texcoord\n", x_texcoord);
	//printf("{%d}{%d} tex config\n", all->tex[0].width, all->tex[0].height);

	int i = (sprite_height > all->win_height) ? (int)(-pos.y) : 0;
	pos.y = (pos.y < 0) ? 0 : pos.y;
	//printf("{%d column %f pos.y {%d - i} \n", column_height, pos.y, i);
	 
	while(i < sprite_height)
	{
		color = get_tex_color(&all->tex[4], x_texcoord, y_texcoord);
		y_texcoord = (i++ * 64) / sprite_height;
		if(pos.y > 0 && pos.y < all->win_height && color != 0)  // есть момент с тем, лишниими циклами при высоких стенах
			my_mlx_pixel_put(&all->screen, pos.x, pos.y, color);
		else if(pos.y > all->win_height)
			break;
	
		pos.y++;

	}

	return(0);
}

int		ft_draw_player(t_all *all) 
{
	t_plr		*plr;
	t_ray		ray;
	t_sprite	sprite;
	
	sprite.distance = -1.;
	ray.num_ray  = 0;
	plr = &all->plr;
	//printf("{%d}plr dir\n", plr->dir);
	ray.dir = plr->dir - 60;
	while(ray.dir < plr->dir + 60)
	{
		ray.len_ray = 0;
		ray.pos.x = plr->pos.x;
		ray.pos.y = plr->pos.y;
		while(all->map_arr[(int)ray.pos.y / (int)all->map_size.y][(int)ray.pos.x / (int)all->map_size.x] != '1')
		{
			if(all->map_arr[(int)ray.pos.y / (int)all->map_size.y][(int)ray.pos.x / (int)all->map_size.x] == '2')
			{
				sprite.pos.x = ray.pos.x;
				sprite.pos.y = ray.pos.y;
				sprite.distance = ray.len_ray;
				sprite.num_ray = ray.num_ray;
			}
			ray.pos.x = plr->pos.x + ray.len_ray * cos(ray.dir * M_PI_2/180);
			ray.pos.y = plr->pos.y + ray.len_ray * sin(ray.dir * M_PI_2/180);
			//my_mlx_pixel_put(&all->screen, ray.pos.x, ray.pos.y, 0x80ff00);
			ray.len_ray += 1; //именно из за этого шага у меня полоса с краев стен!!! изменить систему шага
		}
		if(all->map_arr[(int)ray.pos.y / (int)all->map_size.y][(int)ray.pos.x / (int)all->map_size.x] == '1')
			ft_draw_wall(all, ray);
		ray.num_ray++;
		ray.dir += 120. / all->win_width;
	}

	if(sprite.distance > 0) // будем рисовать спрайты после пускания лучей!))) 
		//printf("{%f x}{%f y}{%f dist}{%d numray}\n", sprite.pos.x, sprite.pos.y, sprite.distance, sprite.num_ray);
		ft_draw_sprite(all, sprite);
	return(0);
}

