/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_mac.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/27 11:40:26 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/24 18:17:30 by ocalamar         ###   ########.fr       */
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
	screen->img = mlx_new_image(all->win.ptr, all->win_width, all->win_height);
	screen->addr = mlx_get_data_addr(screen->img, &screen->bpp,
	&screen->line_len, &screen->endian);
	
	ft_draw_back(all);
	ft_draw_player(all);
	//ft_draw_map(all);
	
	mlx_put_image_to_window(all->win.ptr, all->win.win, screen->img, 0, 0);
	mlx_destroy_image(all->win.ptr, screen->img);
}

void	ft_init_map_size(t_all *all)
{
	char	**map;
	int		map_width;
	int		map_height;

	map_width = 0;
	map_height = 0;
	map = all->map_arr;
	while(map[map_height])
	{
		map_width = MAX(map_width, ft_strlen(map[map_height++]));
	}
	//printf("{%d}width map; {%d} map_height\n", map_width, map_height);
	all->map_size.x = map_width;
	all->map_size.y = map_height;
}

int		ft_init_config(t_all *all)
{
	// парсим ширину высоту экрана//

	all->win_height = 600;
	all->win_width = 800;
	
	all->map_size.x = (int)((all->win_width / all->map_size.x) * MAP_SCALE);
	all->map_size.y = (int)((all->win_height / all->map_size.y) * MAP_SCALE);

	
	return(0);
	
}

int		ft_init_textures(t_all *all)
{
	//t_tex	*tex;
	char	*path = "./eagle.xpm";

	//printf("{%d}{%d} tex config\n", all->tex[0].width, all->tex[0].height);

	all->tex[0].tex = mlx_xpm_file_to_image(all->win.ptr, "./eagle.xpm", &all->tex[0].width, &all->tex[0].height);
	all->tex[0].ptr = mlx_get_data_addr(all->tex[0].tex, &all->tex[0].bpp, &all->tex[0].size_line, &all->tex[0].endian);
	// printf("{%d}{%d} tex config\n", all->tex[0].width, all->tex[0].height);
	return (0);
}
int		main(void)
{
	//принты для проверок
	//printf("{%f}{%f}{%d} in in update x y and dir pos\n", all.plr.pos.x, all.plr.pos.y, all.plr.dir);

	t_all	all;

	all.map_arr = ft_read_map();
	ft_init_map_size(&all);
	ft_init_config(&all);

	ft_init_player(&all);

	all.win.ptr = mlx_init();
	all.win.win = mlx_new_window(all.win.ptr, all.win_width, all.win_height, "Hello world!");

	//---------------------------------//

	ft_init_textures(&all);

	//------------------------------------------------------------//
	
	update_screen(&all);

	//------------------------------------------------------------//

	mlx_hook(all.win.win, 2, 1L<<0, key_press, &all);
	mlx_hook(all.win.win, 3, 1L<<1, key_release, &all);
	mlx_loop_hook(all.win.ptr, &main_loop, &all);
	mlx_loop(all.win.ptr);
	return (0);
}



