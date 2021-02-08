/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/08 17:33:00 by ocalamar         ###   ########.fr       */
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

								 
	while(x++ < 300)
	{
		y = 0;
		while (y++ < 300)
			my_mlx_pixel_put(&img, x, y, 0x00FFFFFF);
	}
	
	my_mlx_pixel_put(&img, 150, 150, 0x0);

	float R = 0.;

	while(R <= 10.)
	{
		x = 150 - R;
		y = 0;
		while (x <= (150 + R))
		{
			y = sqrt(R*R - ((x - 150)*(x - 150))) + 150;
			my_mlx_pixel_put(&img, x, y, 0x99ff00);
			y = -sqrt(R*R - ((x - 150)*(x - 150))) + 150;
			my_mlx_pixel_put(&img, x, y, 0x99ff00);
			x++;
		}
		R += 0.08;
	}

	// int 	start, end = {0};
	// int		a = 0;
	int		x_new, y_new = {0};
	double	a = 180;
	int i = 0;
	
	R = 10.;
	while(R <= 100)
	{
		x = 150 - R * cos(M_PI / 6); 
		while (x <= (150 + R * cos(M_PI / 6)))
		{
			y = sqrt(R*R - ((x - 150)*(x - 150))) + 150;
			//printf("{%d} - x; {%d} - y\n", x, y);
			x_new = (x - 150) * cos(M_PI/180 * a) - (y - 150) * sin(M_PI/180 * a);
			y_new = (x - 150) * sin(M_PI/180 * a) + (y - 150) * cos(M_PI/180 * a);
			x_new += 150;
			y_new += 150;
				//printf("{%d} - x_new; {%d} - y_new\n", x_new, y_new);
			my_mlx_pixel_put(&img, x_new, y_new, 0x0);
			x++;
		}
		R += 0.5;
	}

	mlx_put_image_to_window(mlx, mlx_win, img.img, 50, 50);

	
	// img.img = mlx_new_image(mlx, 300, 300);
	// img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,  &img.endian);
	
	// x = 0;
	// y = 0;
	
	// while(x++ < 299)
	// {
	// 	y = 0;
	// 	while (y++ < 299)
	// 		my_mlx_pixel_put(&img, x, y, 0xFF000000);
	// }

	// x = 0;
	// y = 0;
	// while(x++ < 100)
	// {
	// 	y = 50;
	// 	while (y++ < 100)
	// 		my_mlx_pixel_put(&img, x, y, 0x00FFFF00);
	// }
	
    // mlx_put_image_to_window(mlx, mlx_win, img.img, 100, 100);
	mlx_loop(mlx);
}
