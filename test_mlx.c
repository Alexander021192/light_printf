/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/01/27 14:30:22 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
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
    t_vars    vars;
	void 	*img;
	char *relative_path = "./weapons.xpm";
	
    vars.mlx = mlx_init();
	img = mlx_xpm_file_to_image(vars.mlx, relative_path, 100, 100);
    vars.win = mlx_new_window(vars.mlx, 640, 480, "Hello world!");
    // mlx_hook(vars.win, 2, 1L<<0, key_hook, &vars);
	
	// mlx_hook(vars.win, 3, 1L<<1, key_hook3, &vars);
	
	// mlx_hook(vars.win, 6, 1L<<2, key_hook2, &vars);

	
    mlx_loop(vars.mlx);
} 
