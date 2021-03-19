/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandr <alexandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:11:29 by ocalamar          #+#    #+#             */
/*   Updated: 2021/03/17 18:17:12 by alexandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

int			ft_init_textures(t_all *all)
{
	int		i;
	char	*tex_path;

	i = 0;
	while (i < 5)
	{
		tex_path = all->tex[i].path;
		if ((all->tex[i].tex = mlx_xpm_file_to_image(all->win.ptr, tex_path,
		&all->tex[i].width, &all->tex[i].height)))
			all->tex[i].ptr = mlx_get_data_addr(all->tex[i].tex,
			&all->tex[i].bpp, &all->tex[i].size_line, &all->tex[i].endian);
		else
			return (printf("dont load texture\n"));
		i++;
	}
	return (0);
}

void		ft_init_map_size(t_all *all)
{
	char	**map;
	int		map_width;
	int		map_height;

	map_width = 0;
	map_height = 0;
	map = all->map;
	while (map[map_height])
		map_width = MAX(map_width, ft_strlen(map[map_height++]));
	all->win_width += all->win_width % 64;
	all->map_size.x = ((all->win_width / all->map_size.x) * MAP_SCALE);
	all->map_size.y = ((all->win_height / all->map_size.y) * MAP_SCALE);
}

void		ft_reset_pos_sprites(t_sprite **sprites, t_all *all)
{
	t_sprite	*tmp;

	tmp = *sprites;
	while (*sprites)
	{
		all->MAP(tmp->pos.y, tmp->pos.x) = '2';
		tmp = (*sprites)->next;
		free(*sprites);
		*sprites = tmp;
	}
	*sprites = NULL;
}

int			add_front_sprite(t_sprite **sprites, t_all *all, t_ray ray)
{
	t_sprite	*new;
	t_point		plr_pos;

	if (!(new = (t_sprite*)malloc(sizeof(*new))))
		return (0);
	plr_pos.x = all->plr.pos.x;
	plr_pos.y = all->plr.pos.y;
	new->pos.x = ((int)(ray.pos.x) + 0.5);
	new->pos.y = ((int)(ray.pos.y) + 0.5);
	all->map[(int)new->pos.y][(int)new->pos.x] = '!';
	new->dir = atan2(new->pos.y - plr_pos.y, new->pos.x - plr_pos.x)
	* 180 / M_PI;
	new->dist = sqrt(pow(plr_pos.x - new->pos.x, 2)
	+ pow(plr_pos.y - new->pos.y, 2));
	new->dist *= cos((all->plr.dir - new->dir) * M_PI / 180);
	new->size.y = all->win_height / new->dist;
	new->size.x = new->size.y * (0.75 / ((double)all->win_height
	/ (double)all->win_width));
	new->next = *sprites;
	*sprites = new;
	return (1);
}

t_sprite	*ft_sort_list(t_sprite *root)
{
	t_sprite	*new_root;
	t_sprite	*node;
	t_sprite	*current;

	new_root = NULL;
	while (root != NULL)
	{
		node = root;
		root = root->next;
		if (new_root == NULL || node->dist > new_root->dist)
		{
			node->next = new_root;
			new_root = node;
		}
		else
		{
			current = new_root;
			while (current->next != NULL && !(node->dist > current->next->dist))
				current = current->next;
			node->next = current->next;
			current->next = node;
		}
	}
	return (new_root);
}

int			get_tex_color(t_tex *tex, int x, int y)
{
	return (*(int*)(tex->ptr + (tex->size_line * y) + (tex->bpp / 8 * x)));
}

int			key_press(int keycode, t_all *all)
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

int			key_release(int keycode, t_all *all)
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
		mlx_destroy_window(all->win.ptr, all->win.win);
		exit(0);
	}
	return (0);
}

int			y_update_pos(t_all *all, int flag)
{
	t_point	old_pos;
	t_point	new_pos;
	t_plr	*plr;
	double	dir;

	plr = &all->plr;
	old_pos.x = plr->pos.x;
	old_pos.y = plr->pos.y;
	dir = plr->dir * (M_PI / 180) + M_PI_2;
	new_pos.y = old_pos.y + flag * SPEED;
	plr->pos.x = -(new_pos.y - old_pos.y) * sin(dir) + old_pos.x;
	plr->pos.y = (new_pos.y - old_pos.y) * cos(dir) + old_pos.y;
	if (all->MAP(plr->pos.y, plr->pos.x) == '1' ||
	all->MAP(plr->pos.y, plr->pos.x) == '2')
	{
		plr->pos.x = old_pos.x;
		plr->pos.y = old_pos.y;
		return (0);
	}
	return (1);
}

int			x_update_pos(t_all *all, int flag)
{
	t_point	old_pos;
	t_point	new_pos;
	t_plr	*plr;
	double	dir;

	plr = &all->plr;
	old_pos.x = plr->pos.x;
	old_pos.y = plr->pos.y;
	dir = plr->dir * (M_PI / 180) + M_PI_2;
	new_pos.x = old_pos.x + flag * SPEED;
	plr->pos.x = (new_pos.x - old_pos.x) * cos(dir) + old_pos.x;
	plr->pos.y = (new_pos.x - old_pos.x) * sin(dir) + old_pos.y;
	if (all->MAP(plr->pos.y, plr->pos.x) == '1' ||
	all->MAP(plr->pos.y, plr->pos.x) == '2')
	{
		plr->pos.x = old_pos.x;
		plr->pos.y = old_pos.y;
	}
	return (1);
}

int			rotate_camera(t_all *all, int flag)
{
	all->plr.dir = all->plr.dir + flag * 2;
	all->plr.dir -= (all->plr.dir > 360.) ? 360 : 0;
	all->plr.dir += (all->plr.dir < 0.) ? 360 : 0;
	return (1);
}

int			main_loop(t_all *all)
{
	int	update;

	update = 0;
	if (all->y_move.x || all->y_move.y)
		update = y_update_pos(all, (all->y_move.x) ? -1 : 1);
	if (all->x_move.x || all->x_move.y)
		update = x_update_pos(all, (all->x_move.x) ? -1 : 1);
	if (all->rotate.x || all->rotate.y)
		update = rotate_camera(all, (all->rotate.x) ? -1 : 1);
	if (update == 1)
		update_screen(all);
	return (0);
}

void		my_mlx_pixel_put(t_image *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
	*(unsigned int*)dst = color;
}

void		ft_draw_my_pixel(t_all *all, t_point point,
t_point pxl_size, int color)
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

int			ft_init_player(t_all *all)
{
	t_plr	plr;
	t_point	point;

	ft_bzero(&plr.pos, sizeof(t_point));
	ft_bzero(&point, sizeof(t_point));
	ft_bzero(&all->x_move, sizeof(t_point));
	ft_bzero(&all->y_move, sizeof(t_point));
	ft_bzero(&all->rotate, sizeof(t_point));
	while (all->map[(int)point.y])
	{
		point.x = 0;
		while (all->MAP(point.y, point.x))
		{
			if (ft_strchr("SWNE", all->MAP(point.y, point.x)))
			{
				plr.pos.x = point.x + 0.5;
				plr.pos.y = point.y + 0.5;
				if (all->MAP(point.y, point.x) == 'E')
					plr.dir = 0;
				else if (all->MAP(point.y, point.x) == 'S')
					plr.dir = 90;
				else if (all->MAP(point.y, point.x) == 'W')
					plr.dir = 180;
				else if (all->MAP(point.y, point.x) == 'N')
					plr.dir = 270;
				all->plr = plr;
				return (0);
			}
			point.x++;
		}
		point.y++;
	}
	return (1);
}

int			ft_draw_map(t_all *all)
{
	t_point	point;

	ft_bzero(&point, sizeof(t_point));
	while (all->map[(int)point.y])
	{
		point.x = 0;
		while (all->MAP(point.y, point.x))
		{
			if (all->MAP(point.y, point.x) == '1')
				ft_draw_my_pixel(all, point, all->map_size, 0xffffff);
			point.x++;
		}
		point.y++;
	}
	return (0);
}

int			ft_draw_back(t_all *all)
{
	t_point	sky_floor;

	ft_bzero(&sky_floor, sizeof(t_point));
	while (sky_floor.y < all->win_height / 2)
	{
		sky_floor.x = 0;
		while (sky_floor.x < all->win_width)
			my_mlx_pixel_put(&all->screen, sky_floor.x++,
			sky_floor.y, all->ceil_color);
		sky_floor.y++;
	}
	while (sky_floor.y < all->win_height)
	{
		sky_floor.x = 0;
		while (sky_floor.x < all->win_width)
			my_mlx_pixel_put(&all->screen, sky_floor.x++,
			sky_floor.y, all->floor_color);
		sky_floor.y++;
	}
	return (0);
}

int			ft_draw_wall(t_all *all, t_ray ray)
{
	int		i;
	int		column_height;
	int		num_tex;
	int		x_texcoord;
	int		y_texcoord;
	double	hitx;
	t_point	pos;

	num_tex = 0;
	column_height = (all->win_height / (ray.len_ray *
	cos((ray.dir - all->plr.dir) * (M_PI) / 180)));
	//-----------//func get number tex-------------///
	ray.dir -= (ray.dir > 360) ? 360 : 0;
	if (ray.side == 0)
	{
		if (ray.dir > 90 && ray.dir < 270)
			num_tex = 2;
		else
			num_tex = 3;
	}
	else
	{
		if (ray.dir > 0 && ray.dir < 180)
			num_tex = 1;
		else
			num_tex = 0;
	}
	// -------------------------------------------///
	//----------func get x_texcoord ----------//
	if (ray.side == 0)
		hitx = all->plr.pos.y + ray.len_ray * (sin(ray.dir * M_PI / 180));
	else
		hitx = all->plr.pos.x + ray.len_ray * (cos(ray.dir * M_PI / 180));
	hitx -= floor(hitx);
	x_texcoord = (1 - hitx) * all->tex[num_tex].width;
	if (num_tex == 0 || num_tex == 3)
		x_texcoord = (hitx) * all->tex[num_tex].width;
	// -----------------------------//
	y_texcoord = 0;
	pos.x = ray.num_ray;
	pos.y = (all->win_height / 2 - column_height / 2);
	i = (column_height > all->win_height) ? (int)(-pos.y) : 0;
	pos.y = (pos.y < 0) ? 0 : pos.y;
	while (i < column_height)
	{
		y_texcoord = (i++ * all->tex[num_tex].height) / column_height;
		if (pos.y > 0 && pos.y < all->win_height)
			my_mlx_pixel_put(&all->screen, pos.x, pos.y,
				get_tex_color(&all->tex[num_tex], x_texcoord, y_texcoord));
		else if (pos.y > all->win_height)
			break ;
		pos.y++;
	}
	return (0);
}

int			ft_draw_sprites(t_all *all, t_sprite *sprite, double arr_len_ray[])
{
	while (sprite)
	{
		ft_draw_sprite(all, sprite, arr_len_ray);
		sprite = sprite->next;
	}
	return (0);
}

int			ft_draw_sprite(t_all *all, t_sprite *sprite, double arr_len_ray[])
{
	double	abs_dir;
	int		h_offset;
	int		v_offset;
	int		i;
	int		j;
	int		color;

	abs_dir = sprite->dir - all->plr.dir;
	if (abs_dir > 40)
		abs_dir -= 360;
	else if (abs_dir < -40)
		abs_dir += 360;
	h_offset = (abs_dir) / 60 * all->win_width +
	all->win_width / 2 - sprite->size.x / 2;
	v_offset = all->win_height / 2 - sprite->size.y / 2;
	i = -1;
	j = -1;
	while (i++ < sprite->size.x)
	{
		if (h_offset + i < 0 || h_offset + i >= all->win_width)
			continue;
		if (arr_len_ray[h_offset + i] < sprite->dist)
			continue;
		while (j++ < sprite->size.y - 2)
		{
			if (v_offset + j < 0 || v_offset + j >= all->win_height - 5)
				continue;
			color = get_tex_color(&all->tex[4], i * all->tex[4].width
			/ sprite->size.x, j * all->tex[4].height / sprite->size.y);
			if (color)
				my_mlx_pixel_put(&all->screen, all->win_width + h_offset + i,
				v_offset + j, color);
		}
		j = -1;
	}
	return (0);
}

int			ft_init_ray(t_all *all, t_ray *ray)
{
	t_point	plr_pos;

	plr_pos.x = all->plr.pos.x;
	plr_pos.y = all->plr.pos.y;
	ray->pos.x = (int)plr_pos.x;
	ray->pos.y = (int)plr_pos.y;
	if (sin(ray->dir * M_PI / 180) == 0)
		ray->delta_dist.x = 0;
	else if (cos(ray->dir * M_PI / 180) == 0)
		ray->delta_dist.x = 1;
	else
		ray->delta_dist.x = ABS(1 / cos(ray->dir * M_PI / 180));
	if (cos(ray->dir * M_PI / 180) == 0)
		ray->delta_dist.y = 0;
	else if (sin(ray->dir * M_PI / 180) == 0)
		ray->delta_dist.y = 1;
	else
		ray->delta_dist.y = ABS(1 / sin(ray->dir * M_PI / 180));
	if (cos(ray->dir * M_PI / 180) < 0)
	{
		ray->step.x = -1;
		ray->side_dist.x = (plr_pos.x - (int)plr_pos.x) * ray->delta_dist.x;
	}
	else
	{
		ray->step.x = 1;
		ray->side_dist.x = ((int)plr_pos.x + 1. - plr_pos.x) *
		ray->delta_dist.x;
	}
	if (sin(ray->dir * M_PI / 180) < 0)
	{
		ray->step.y = -1;
		ray->side_dist.y = (plr_pos.y - (int)plr_pos.y) * ray->delta_dist.y;
	}
	else
	{
		ray->step.y = 1;
		ray->side_dist.y = ((int)plr_pos.y + 1. - plr_pos.y) *
		ray->delta_dist.y;
	}
	return (0);
}

void		ft_dda_step(t_ray *ray)
{
	if (ray->side_dist.x < ray->side_dist.y)
	{
		ray->side_dist.x += ray->delta_dist.x;
		ray->pos.x += ray->step.x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist.y += ray->delta_dist.y;
		ray->pos.y += ray->step.y;
		ray->side = 1;
	}
}

double		ft_get_len_ray(t_all *all, t_ray *ray)
{
	double len_ray;

	len_ray = 0;
	if (ray->side == 0)
		len_ray = (ray->pos.x - all->plr.pos.x + (1 - ray->step.x) / 2)
		/ (cos(ray->dir * M_PI / 180));
	else
		len_ray = (ray->pos.y - all->plr.pos.y + (1 - ray->step.y) / 2)
		/ (sin(ray->dir * M_PI / 180));
	ray->len_ray = len_ray;
	return (len_ray);
}

int			ft_draw_player(t_all *all)
{
	t_plr		*plr;
	t_ray		ray;
	t_sprite	*sprites;
	double		arr_len_ray[all->win_width];

	sprites = NULL;
	ray.num_ray = 0;
	plr = &all->plr;
	ray.dir = plr->dir - 30;
	while (ray.dir < plr->dir + 30)
	{
		ft_init_ray(all, &ray);
		while (all->MAP(ray.pos.y, ray.pos.x) != '1')
		{
			ft_dda_step(&ray);
			if (all->MAP(ray.pos.y, ray.pos.x) == '2')
				if (!add_front_sprite(&sprites, all, ray))
					return (printf("\n exit game \n"));
		}
		if (all->MAP(ray.pos.y, ray.pos.x) == '1')
		{
			arr_len_ray[ray.num_ray] = ft_get_len_ray(all, &ray);
			ft_draw_wall(all, ray);
		}
		ray.num_ray += 1;
		ray.dir += 60. / all->win_width;
	}
	if (sprites)
	{
		sprites = ft_sort_list(sprites);
		ft_draw_sprites(all, sprites, arr_len_ray);
		ft_reset_pos_sprites(&sprites, all);
	}
	return (0);
}
