/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/16 17:09:33 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/mlx.h"
#include "./includes/cub.h"
#include "./includes/libft.h"
#include "./includes/utils.h"

#include <stdio.h>

void	update_screen(t_all *all)
{
	t_image		*screen;

	screen = &all->screen;
	screen->img = mlx_new_image(all->win.ptr, 640, 480);
	screen->addr = mlx_get_data_addr(screen->img, &screen->bpp,
	&screen->line_len, &screen->endian);
	
	
	ft_draw_map(all);
	
	ft_draw_player(all);
	
	mlx_put_image_to_window(all->win.ptr, all->win.win, screen->img, 0, 0);
	mlx_destroy_image(all->win.ptr, screen->img);
}

int		main(void)
{
	//принты для проверок
	//printf("{%f}{%f}{%d} in in update x y and dir pos\n", all.plr.pos.x, all.plr.pos.y, all.plr.dir);


	t_all	all;

	all.map_arr = ft_read_map();

	ft_init_player(&all);

	all.win.ptr = mlx_init();
	all.win.win = mlx_new_window(all.win.ptr, 640, 480, "Hello world!");
	
	//------------------------------------------------------------//

	update_screen(&all);

	//------------------------------------------------------------//

	mlx_hook(all.win.win, 2, 1L<<0, key_press, &all);
	mlx_hook(all.win.win, 3, 1L<<1, key_release, &all);
	mlx_loop_hook(all.win.ptr, &main_loop, &all);
	mlx_loop(all.win.ptr);
	return (0);
}



