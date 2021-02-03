/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/03 16:15:07 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
#include <math.h>
#include <stdio.h>

int		rgb_to_hex(int t, int r, int g, int b)
{
	return(t << 24 | r << 16 | g << 8 | b);
}

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}



typedef struct  s_vars {
    void        *mlx;
    void        *win;
}               t_vars;

int             key_hook(int keycode, t_vars *vars)
{
	if (keycode == 53)
    	mlx_destroy_window(vars->mlx, vars->win);
	else //можем печатать номера печаемых кнопок
		printf(" you press button number {%d}\n", keycode);
	return (0);
}

int             key_hook2(int keycode_x, int keycode_y,  t_vars *vars)
{ 
	// мышь показывает ее координаты
	printf(" you press button number {%d}{%d}\n", keycode_x, keycode_y);
	return (0);
}

int             key_hook3(int keycode,  t_vars *vars)
{ 
	// мышь показывает ее координаты
	printf(" you press-release button number {%d}\n", keycode);
	return (0);
}

int             main(void)
{
    void    *mlx;
    void    *mlx_win;
    t_data  img;

	int x, y = {0};

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 640, 480, "Hello world!");
    img.img = mlx_new_image(mlx, 400, 400);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
	
	while(x++ < 350)
	{
		y = 0;
		while (y++ < 350)
			my_mlx_pixel_put(&img, sqrt(y*y - 100*100), sqrt(x*x - 100*100), 0x00FF0000);
	}
	
    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);

	long long int i = 0;

	img.img = mlx_new_image(mlx, 400, 400);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);

	x = 0;
	y = 0;
	while(x++ < 100)
	{
		y = 50;
		while (y++ < 100)
			my_mlx_pixel_put(&img, x, y, 0x80FFFF00);
	}
	
    mlx_put_image_to_window(mlx, mlx_win, img.img, 0, 0);
	mlx_loop(mlx);
}
