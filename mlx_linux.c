/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandr <alexandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/15 16:41:23 by alexandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
#include "./includes/libft.h"
#include <math.h>
#include <stdio.h>

#define SCALE 16
#define SPEED 0.5

#define KEY_Q			113
#define KEY_W			119
#define KEY_E			101
#define KEY_A			97
#define KEY_S			115
#define KEY_D			100
#define KEY_LEFT		65363
#define KEY_RIGHT		65361
#define KEY_FORWARD 	65362
#define KEY_BACKWARD	65364
#define KEY_ESC			65307

typedef	struct s_point
{
	double x;
	double y;
}				t_point;

typedef struct	s_data {
	void	*ptr;
	void	*win;
	char	**map;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		dir;
	t_point	pos;
	t_point	x_move;
	t_point	y_move;
	t_point	rotate;
}				t_data;

char	**ft_read_map(void);
void	update_screen(t_data *data);

int		rgb_to_hex(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

int		key_press(int keycode, t_data *data)
{
	//printf("%d in press\n",keycode);
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		data->y_move.x = 1;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		data->y_move.y = 1;
	if (keycode == KEY_D)
		data->x_move.y = 1;
	else if (keycode == KEY_A)
		data->x_move.x = 1;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		data->rotate.x = 1;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		data->rotate.y = 1;
	return (0);
}

int		key_release(int keycode, t_data *data)
{
	if (keycode == KEY_W || keycode == KEY_FORWARD)
		data->y_move.x = 0;
	else if (keycode == KEY_S || keycode == KEY_BACKWARD)
		data->y_move.y = 0;
	if (keycode == KEY_D)
		data->x_move.y = 0;
	else if (keycode == KEY_A)
		data->x_move.x = 0;
	if (keycode == KEY_Q || keycode == KEY_LEFT)
		data->rotate.x = 0;
	else if (keycode == KEY_E || keycode == KEY_RIGHT)
		data->rotate.y = 0;
	else if (keycode == KEY_ESC)
	{
		printf("window closed\n");
		mlx_destroy_window(data->ptr, data->win);	
	}
	return (0);
}

int		y_update_pos(t_data *data, int flag)
{
	t_point	old_pos;
	t_point new_pos;
	double	dir;
	
	old_pos.x = data->pos.x;
	old_pos.y = data->pos.y;
	dir = data->dir * (M_PI_2 / 180) + M_PI_2;
	printf("{%f} - x; {%f} - y; {%d} - dir: pos\n", data->pos.x, data->pos.y, data->dir);
	
	new_pos.y = old_pos.y + flag * SPEED;
	//data->pos.y = new_pos.y;
	data->pos.x = - (new_pos.y - old_pos.y) * sin(dir) + old_pos.x;
	data->pos.y = (new_pos.y - old_pos.y) * cos(dir) + old_pos.y;
	if(data->map[(int)data->pos.y/SCALE][(int)data->pos.x/SCALE] == '1')
	{
		data->pos.x = old_pos.x;
		data->pos.y = old_pos.y;
		return (0);
	}
	
	return(1);	
}

int		x_update_pos(t_data *data, int flag)
{
	t_point	old_pos;
	t_point new_pos;
	double	dir;
	
	old_pos.x = data->pos.x;
	old_pos.y = data->pos.y;
	dir = data->dir * (M_PI_2 / 180) + M_PI_2;

	
	new_pos.x = old_pos.x + flag * SPEED;
	data->pos.x = (new_pos.x - old_pos.x) * cos(dir) + old_pos.x;
	data->pos.y = (new_pos.x - old_pos.x) * sin(dir) + old_pos.y;
	
	if(data->map[(int)data->pos.y/SCALE][(int)data->pos.x/SCALE] == '1')
	{
		data->pos.x = old_pos.x;
		data->pos.y = old_pos.y;
		return (0);
	}
	
	return(1);	
}

int		rotate_camera(t_data *data, int flag)
{
	data->dir = data->dir % 720 + flag;
	return (1);
}

int		main_loop(t_data *data)
{
	int	update = 0;
	if (data->y_move.x || data->y_move.y)
		//printf("move up/down\n");
		update = y_update_pos(data, (data->y_move.x) ? -1 : 1);
	if (data->x_move.x || data->x_move.y)
		//printf("move right/left\n");
		update = x_update_pos(data, (data->x_move.x) ? -1 : 1);
	 if (data->rotate.x || data->rotate.y)
	 	//printf("move rotate\n");
		update = rotate_camera(data, (data->rotate.x) ? -1 : 1);
	if(update == 1)
		update_screen(data);
	return(0);
}

void	set_pos(t_point *pos, double x, double y)
{
	pos->x = x;
	pos->y = y;
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	ft_draw_my_pixel(t_data *data, t_point point, int color)
{
	t_point	end;
	
	end.x = (point.x + 1) * SCALE;
	end.y = (point.y + 1) * SCALE;
	point.x *= SCALE;
	point.y *= SCALE;

	while (point.y < end.y)
	{
		while (point.x < end.x)
			my_mlx_pixel_put(data, point.x++, point.y, color);
		point.x -=SCALE;
		point.y++;
	}
}

int		ft_init_player(t_data *data)
{
	data->pos.x = 0;
	data->pos.y = 0;
	ft_bzero(&data->pos, sizeof(t_point));
	ft_bzero(&data->x_move, sizeof(t_point));
	ft_bzero(&data->y_move, sizeof(t_point));
	ft_bzero(&data->rotate, sizeof(t_point));
	return(0);
}

int		ft_draw_map(t_data *data)
{
	t_point		point;
		
	ft_bzero(&point, sizeof(t_point));
	
	while(data->map[(int)point.y])
	{
		point.x = 0;
		while (data->map[(int)point.y][(int)point.x])
		{
			if(data->map[(int)point.y][(int)point.x] == '1')
				//mlx_pixel_put(win->mlx, win->win, point.x, point.y, 0xffffff);
				ft_draw_my_pixel(data, point, 0xffffff);
			else if((data->pos.x == 0 && data->pos.y == 0) &&
			ft_strchr("SWNE", data->map[(int)point.y][(int)point.x]))
			{
				data->pos.x = point.x * SCALE + SCALE/2;
				data->pos.y = point.y * SCALE + SCALE/2;
				if(data->map[(int)point.y][(int)point.x] == 'E')
					data->dir = 0;
				else if(data->map[(int)point.y][(int)point.x] == 'S')
					data->dir = 180;
				else if(data->map[(int)point.y][(int)point.x] == 'W')
					data->dir = 360;
				else if (data->map[(int)point.y][(int)point.x] == 'N')
					data->dir = 540;
			}
			point.x++;
		}
		point.y++;
	}
	return (0);
}

int		ft_draw_player(t_data *data)
{
	double	start = data->dir - 60;
	t_point	pos;

	while(start < data->dir + 60)
	{
		pos.x = data->pos.x;
		pos.y = data->pos.y;
		while(data->map[(int)pos.y/SCALE][(int)pos.x/SCALE] != '1')
		{
			pos.x += cos(start * M_PI_2/180);
			pos.y += sin(start * M_PI_2/180);
			my_mlx_pixel_put(data, pos.x, pos.y, 0x80ff00);
		}
		start += 0.5;
	}
	return(0);
}

void	update_screen(t_data *data)
{
	data->img = mlx_new_image(data->ptr, 640, 480);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
	&data->line_length, &data->endian);
	
	ft_draw_map(data);
	
	
	
	ft_draw_player(data);
	
	mlx_put_image_to_window(data->ptr, data->win, data->img, 0, 0);
	mlx_destroy_image(data->ptr, data->img);
}

int		main(void)
{
	t_data	data;

	data.map = ft_read_map();
	ft_init_player(&data);
	
	data.ptr = mlx_init();
	data.win = mlx_new_window(data.ptr, 640, 480, "Hello world!");
	//------------------------------------------------------------//

	update_screen(&data);

	//------------------------------------------------------------//
	mlx_hook(data.win, 2, 1L<<0, key_press, &data);
	mlx_hook(data.win, 3, 1L<<1, key_release, &data);
	mlx_loop_hook(data.ptr, &main_loop, &data);
	mlx_loop(data.ptr);
}
