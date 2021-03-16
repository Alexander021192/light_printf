/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 12:21:43 by alexandr          #+#    #+#             */
/*   Updated: 2021/03/16 16:47:41 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

char	*ft_cut_space(char *str)
{
	int		count_space;
	char	*line_without_space;
	int		i;

	i = 0;
	count_space = 0;
	while(str && str[i++])
	{
		if(str[i] == ' ' && (str[i + 1] && str[i + 1] != ' '))
			count_space++;
	}
	//printf("{%d} space count str {%s}\n", count_space, str);
	line_without_space = malloc(ft_strlen(str) - count_space);
	i = 0;
	while(str && *str)
	{
		if( *str != ' ' || ((*str == ' ' && str[1] && str[1] == ' ')))
			line_without_space[i++] = *str;
		str++;
	}
	line_without_space[i] = 0;
	//free(line) ? 
	//printf("{%d} space count str {%s}\n", count_space, line_without_space);
	return(line_without_space);
}

char	**make_map(t_list **begin, int size)
{
	t_list	*tmp = *begin;
	char	**map;
	int		i;

	i = 0;
	map = ft_calloc(size + 1, sizeof(char*));
	while (tmp)
	{
		map[i++] = tmp->content;
		tmp = tmp->next;
	}
	while (*begin)
	{
		tmp = (*begin)->next;
		free(*begin);
		*begin = tmp;
	}
	*begin = NULL;
	return (map);
}

int		ft_get_num(char **str)
{
	int	num = 0;
	if(*str)
		while(!ft_isdigit(**str))
			(*str)++;
	while(ft_isdigit(**str))
	{
		num = num * 10 + (**str - '0');
		(*str)++;
	}
	return num;
}

int		get_resolution(t_all *all, char *str, int* check_settings, int tex_index)
{
	int max_width = 0;
	int	max_height = 0;
	int	win_width = 0;
	int	win_height = 0;

	mlx_get_screen_size(all->win.ptr, &max_width, &max_height);
	win_width = ft_get_num(&str);
	if(win_width > max_width)
		return(printf("error. Win_width too large\n"));
	else if(win_width < MIN_WIN_WIDTH)
		return(printf("error. Win_width too low\n"));

	win_height = ft_get_num(&str);
	if(win_height > max_height)
		return(printf("error. win_height too large\n"));
	else if(win_height < MIN_WIN_HEIGHT)
		return(printf("error. win_height too low\n"));

	all->win_height = win_height;
	all->win_width = win_width;
	check_settings[tex_index] = 1;
	return(0);
}

int		get_texpath(t_all *all, char *str, int* check_settings, int tex_index)
{
	int		fd;
	char	*tex_path;
	tex_path = ft_strtrim(str, " ");
	if(!tex_path)
		return(printf("error with texture path"));
	if((fd = open(tex_path, O_RDONLY) < 0))
		return(printf("error with texture path"));
	close(fd);
	all->tex[tex_index - 1].path = tex_path;
	check_settings[tex_index] = 1;
	return (0);
}

int		create_color(int r, int g, int b)
{
	if(r > 255 || g > 255 || b > 255)
	{
		printf("error with back color\n");
		return(-1);
	}
	return(r << 16 | g << 8 | b);
}

int 	get_backcolor(t_all *all, char *str, int* check_settings, int tex_index)
{
	int color = 0;

	color = create_color(ft_get_num(&str), ft_get_num(&str), ft_get_num(&str));
	if(color >= 0)
	{
		if(tex_index == 6)
		{
			all->floor_color = color;
			check_settings[tex_index] = 1;
		}
		if(tex_index == 7)
		{
			all->ceil_color = color;
			check_settings[tex_index] = 1;
		}
	}
	return(0);
}

int		ft_parse_settings(t_all *all, char *str, int *check_settings)
{
	if(ft_strnstr(str, "R ", 2))
		get_resolution(all, str + 2, check_settings, 0);
	else if(ft_strnstr(str, "NO ", 3))
		get_texpath(all, str + 3, check_settings, 1);
	else if(ft_strnstr(str, "SO ", 3))
		get_texpath(all, str + 3, check_settings, 2);
	else if(ft_strnstr(str, "WE ", 3))
		get_texpath(all, str + 3, check_settings, 3);
	else if(ft_strnstr(str, "EA ", 3))
		get_texpath(all, str + 3, check_settings, 4);
	else if(ft_strnstr(str, "S ", 2))
		get_texpath(all, str + 2, check_settings, 5);
	else if(ft_strnstr(str, "F ", 2))
		get_backcolor(all, str + 2, check_settings, 6);
	else if(ft_strnstr(str, "C ", 2))
		get_backcolor(all, str + 2, check_settings, 7);
	return (0);
}

char	**ft_read_map(t_all *all, char **argv)
{
	int		fd;
	char	*line;
	t_list	*begin;
	
	int	check_settings[8];
	ft_bzero(check_settings, 8);
			
	line = NULL;
	begin = NULL;
	fd = open(argv[1], O_RDONLY);
	while (get_next_line(fd, &line))
	{
		if (ft_strchr("NRSWEFC", *line))
			ft_parse_settings(all, line, check_settings);
		else
			ft_lstadd_back(&begin, ft_lstnew(ft_cut_space(line)));
		free(line);
		line = NULL;
	}
	ft_lstadd_back(&begin, ft_lstnew(ft_cut_space(line)));
	free(line);

	int i = 0;
	while(i < 8)
		if(!check_settings[i++])
			exit(0); // need func exit game
	close(fd);
	return (make_map(&begin, ft_lstsize(begin)));
}

