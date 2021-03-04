/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:11:29 by ocalamar          #+#    #+#             */
/*   Updated: 2021/03/04 17:21:57 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub.h"
#include "./includes/libft.h"
#include "./includes/mlx.h"
#include "./includes/utils.h"

#include <stdio.h>


void	ft_reset_pos_sprites(t_sprite **sprites, t_all *all)
{
	t_sprite	*tmp;
	tmp = *sprites;
	while (*sprites)
	{
		//printf("pos y{%d} x{%d}\n", (int)tmp->pos.y, (int)tmp->pos.x);
		all->map_arr[(int)tmp->pos.y][(int)tmp->pos.x] = '2';
		
		tmp = (*sprites)->next;
		free(*sprites);
		*sprites = tmp;
	}
	*sprites = NULL;
}

int		add_front_sprite(t_sprite **sprites, t_all *all, t_ray ray)
{
	t_sprite	*new;
	t_point		plr_pos;
	
	if (!(new = (t_sprite*)malloc(sizeof(*new))))
		return (0);

	plr_pos.x = (all->plr.pos.x / all->map_size.x);
	plr_pos.y = (all->plr.pos.y / all->map_size.y);
	new->pos.x = ((int)(ray.pos.x) / (int)(all->map_size.x) + 0.5);
	new->pos.y = ((int)(ray.pos.y) / (int)(all->map_size.y) + 0.5);

	//printf("{%.2f}{%.2f} spr pos\n", spr_pos.x, spr_pos.y);
	all->map_arr[(int)new->pos.y][(int)new->pos.x] = '!';
	
	new->dir = atan2(new->pos.y - plr_pos.y, new->pos.x - plr_pos.x) * 180/M_PI_2;
	new->dir = (int)(new->dir + 720) % 720;

	new->dist = sqrt(pow(plr_pos.x - new->pos.x, 2) + pow(plr_pos.y - new->pos.y, 2));
	new->dist *= cos((all->plr.dir - new->dir)/2 * M_PI/180);
	new->size = all->win_height / new->dist;
	//sprite_dist *= cos((sprite_dir - all->plr.dir) * M_PI_2/180);
	// printf("sprite dist{%.3f}\n", sprite_dist * cos((sprite_dir - all->plr.dir) * M_PI/180));
	//printf("sprite dir{%.3f}; dist{%.3f} size{%.3f}\n", new->dir, new->dist, new->size);
	new->size = all->win_height / new->dist;
	new->next = *sprites;
	*sprites = new;
	return (1);
}

t_sprite *ft_sort_list(t_sprite *root)
{
	t_sprite *new_root = NULL;
	t_sprite *node = root;
	t_sprite *current = new_root;
	while ( root != NULL )
	{
		node = root;
		root = root->next;
		if ( new_root == NULL || node->dist > new_root->dist )
		{
			node->next = new_root;
			new_root = node;
		}
		else
		{
			current = new_root;
			while ( current->next != NULL && !( node->dist > current->next->dist ) )
				current = current->next;
			node->next = current->next;
			current->next = node;
		}
	}
	return new_root;
}

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
			
	ray.len_ray = (ray.len_ray ) / all->map_size.y * cos((ray.dir - all->plr.dir) * (M_PI_2)/180);
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

int		ft_draw_sprites(t_all *all, t_sprite *sprite, double arr_len_ray[])
{
	int i = 0;
	while(sprite)
	{
		printf("{%d} - {%.3f}\n",i++ , sprite->dist);
		ft_draw_sprite(all, sprite, arr_len_ray);
		sprite = sprite->next;
	}
	return (0);
}
int		ft_draw_sprite(t_all *all, t_sprite *sprite, double arr_len_ray[])
{
	t_point plr_pos;
	
	plr_pos.x = (all->plr.pos.x / all->map_size.x);
	plr_pos.y = (all->plr.pos.y / all->map_size.y);



	// printf("sprite coord {%.2f}{%.2f}\n", sprite.pos.x, sprite.pos.y);
	// printf("plr coord {%.2f}{%.2f}\n", plr_pos.x, plr_pos.y);
	// double	sprite_dir = atan2(sprite.pos.y - plr_pos.y, sprite.pos.x - plr_pos.x) * 180/M_PI_2;
	// sprite_dir = (int)(sprite_dir + 720) % 720;
	// double	sprite_dist = sqrt(pow(plr_pos.x - sprite.pos.x, 2) + pow(plr_pos.y - sprite.pos.y, 2));
	// sprite_dist *= cos((sprite_dir - all->plr.dir) * M_PI_2/180);
	// printf("sprite dist{%.3f}\n", sprite_dist * cos((sprite_dir - all->plr.dir) * M_PI/180));
	// printf("sprite dist{%.3f}\n", sprite_dist);
	//int	sprite_size = all->win_height / sprite_dist;
	printf("sprite dir {%.2f} and dist {%.2f}, size {%.2f}\n", sprite->dir, sprite->dist, sprite->size);
	
	int		h_offset = (sprite->dir - all->plr.dir) / 120 * all->win_width + all->win_width/2 - 64/2;
	int		v_offset = all->win_height / 2 - sprite->size/2;
	
	printf("{%d}{%d} offset  \n", h_offset, v_offset);

	int i = -1, j = -1;

	while (i++ < sprite->size)
	{
		if(h_offset+i < 0 || h_offset + i >= all->win_width)
			continue;
		//printf("{%.2f ray len {%d}}{%.2f sprdist}\n", arr_len_ray[h_offset+i], h_offset+i ,sprite_dist);
		if(arr_len_ray[h_offset+i] < sprite->dist)
			continue;
		while (j++ < sprite->size - 2)
		{
			if(v_offset+j < 0 || v_offset + j >= all->win_height)
				continue;
			int color = get_tex_color(&all->tex[4], i*64/sprite->size, j*64/sprite->size);
			if(color)
				my_mlx_pixel_put(&all->screen, all->win_width + h_offset + i, v_offset + j, color);
		}
		j = -1;
		
	}
	
	return(0);
}

int		ft_draw_player(t_all *all) 
{
	t_plr		*plr;
	t_ray		ray;
	t_sprite	*sprites;
	double		arr_len_ray[all->win_width];
	
	sprites = NULL;
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
				add_front_sprite(&sprites, all, ray);
				//sprite.pos.x = (int)(ray.pos.x / all->map_size.x) + 0.5; 		// + offset в координаты зависит от угла
																				// здесь мы записываем координаты видимых нами спрайтов
																				// 
																				// будем проверять по координатам, бегая по списку каждый раз до самого конца
																				// если есть список с такими же координатами, то ничего не делаем/
																				// есть нету, то добавляем новый спрайт и вычислим их дистанции для будущей соритировки
				//sprite.pos.y = (int)(ray.pos.y / all->map_size.y) + 0.5;
			}
			ray.pos.x = plr->pos.x + ray.len_ray * cos(ray.dir * M_PI_2/180);
			ray.pos.y = plr->pos.y + ray.len_ray * sin(ray.dir * M_PI_2/180);
			//my_mlx_pixel_put(&all->screen, ray.pos.x, ray.pos.y, 0x80ff00);
			ray.len_ray += 1; //именно из за этого шага у меня полоса с краев стен!!! изменить систему шага
		}
		if(all->map_arr[(int)ray.pos.y / (int)all->map_size.y][(int)ray.pos.x / (int)all->map_size.x] == '1')
		{
			arr_len_ray[ray.num_ray] = sqrt(pow((plr->pos.x - ray.pos.x)/all->map_size.x, 2) + pow((plr->pos.y - ray.pos.y)/all->map_size.y, 2));
;
			ft_draw_wall(all, ray);
		}
		ray.num_ray++;
		ray.dir += 120. / all->win_width;
	}
	if(sprites)
	{
		ft_draw_sprites(all, ft_sort_list(sprites), arr_len_ray);
		ft_reset_pos_sprites(&sprites, all);// и тут же можно очистить sprites
	}
	return(0);
}

