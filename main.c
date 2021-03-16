/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/03/16 17:55:54 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/cub.h"

void	update_screen(t_all *all)
{
	t_image		*screen;

	screen = &all->screen;
	screen->img = mlx_new_image(all->win.ptr, all->win_width, all->win_height); // remove from here init image
	screen->addr = mlx_get_data_addr(screen->img, &screen->bpp,
	&screen->line_len, &screen->endian);
	
	ft_draw_back(all);
	ft_draw_player(all);
	ft_draw_map(all); //почему то перестала рисоваться карта
	
	mlx_put_image_to_window(all->win.ptr, all->win.win, screen->img, 0, 0);
	mlx_destroy_image(all->win.ptr, screen->img);
}

int		main(int argc, char **argv)
{
	(void)argc;
	t_all	all;
	
	all.map = ft_read_map(&all, argv);
	if(check_map(all.map))
		return(printf("\n exit game \n"));
		
	ft_init_map_size(&all);
	ft_init_player(&all);
	
	all.win.ptr = mlx_init();
	all.win.win = mlx_new_window(all.win.ptr, all.win_width, all.win_height, "Hello world, i am cub3D!");

	if(ft_init_textures(&all))
		return(printf("\n exit game \n"));
		
	update_screen(&all);

	mlx_hook(all.win.win, 2, 1L<<0, key_press, &all);
	mlx_hook(all.win.win, 3, 1L<<1, key_release, &all);
	mlx_loop_hook(all.win.ptr, &main_loop, &all);
	mlx_loop(all.win.ptr);
	return (0);
}



