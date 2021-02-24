/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 14:02:58 by ocalamar          #+#    #+#             */
/*   Updated: 2021/02/24 17:55:47 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include "cub.h"

# define MAX(x, y) ((x > y) ? x : y)
# define ABS(x)	((x > 0) ? x : -x)

char	**ft_read_map(void);
void	update_screen(t_all *all);
int		key_press(int keycode, t_all *all);
int		key_release(int keycode, t_all *all);
int		y_update_pos(t_all *all, int flag);
int		x_update_pos(t_all *all, int flag);
int		rotate_camera(t_all *all, int flag);
int		main_loop(t_all *all);
void	set_pos(t_point *pos, double x, double y);
void	my_mlx_pixel_put(t_image *img, int x, int y, int color);
void	ft_draw_my_pixel(t_all *all, t_point point, t_point pxl_size, int color);
int		ft_init_player(t_all *all);
int		ft_draw_map(t_all *all);
int		ft_draw_player(t_all *all);
int		ft_draw_back(t_all *all);
int		load_tex(t_all *all, t_tex *tex, char *path);

#endif