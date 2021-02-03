/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 13:00:10 by alexandr          #+#    #+#             */
/*   Updated: 2021/02/03 17:31:23 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_H
# define CUB_H

#define SCALE 16 // условный размер каждого квадратика в карте

#	include <math.h>

typedef struct	s_win //структура для окна
{
	void		*mlx;
	void		*win;
	void		*img;
	void		*addr;
	int			line_l;
	int			bpp;
	int			en;
}				  t_win;

typedef struct	s_point // структура для точки
{
	float			x;
	float			y;
}				  t_point;

typedef struct	s_plr //структура для игрока и луча
{
	float		x;
	float		y;
	float		dir;
	float		start;
	float		end;
}				  t_plr;

typedef struct	s_all // структура для всего вместе
{
	t_win		*win;
	t_plr		*plr;
	char		**map;
}				  t_all;

typedef	struct		s_pars
{
	char	*resolution;
	char	*n_textures;
	char	*s_textures;
	char	*w_textures;
	char	*e_textures;
	char	*sprt_textures;
	char	*flr_textures;
	char	*cl_textures;
}					t_pars;

t_pars	g_pars;

char	**ft_read_map(void);

#endif