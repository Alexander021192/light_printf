/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 12:21:43 by alexandr          #+#    #+#             */
/*   Updated: 2021/03/11 20:27:37 by ocalamar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"
#include "./includes/cub.h"

#include <stdio.h>

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
	//ft_lstclear(begin, free);
	// i = 0;
	// while (map[i])
	// {
	// 	ft_putendl_fd(map[i], 1);
	// 	i++;
	// }
	return (map);
}

char	*get_texpath(char *str, int* check_settings, int tex_index)
{
	check_settings[tex_index] = 1;
	return(ft_strdup(str));
}

char 	*get_resolution(char *str, int* check_settings, int tex_index)
{
	check_settings[tex_index] = 1;
	return(ft_strdup(str));
}

char 	*get_backcolor(char *str, int* check_settings, int tex_index)
{
	check_settings[tex_index] = 1;
	return(ft_strdup(str));
}

int		ft_parse_settings(t_all *all, char *str, int *check_settings)
{
	if(ft_strnstr(str, "R ", 2))
		g_tex_char.resolution = get_resolution(str + 2, check_settings, 0);
	else if(ft_strnstr(str, "NO ", 3))
		g_tex_char.n_textures = get_texpath(str + 3, check_settings, 1);
	else if(ft_strnstr(str, "SO ", 3))
		g_tex_char.s_textures = get_texpath(str + 3, check_settings, 2);
	else if(ft_strnstr(str, "WE ", 3))
		g_tex_char.w_textures = get_texpath(str + 3, check_settings, 3);
	else if(ft_strnstr(str, "EA ", 3))
		g_tex_char.e_textures = get_texpath(str + 3, check_settings, 4);
	else if(ft_strnstr(str, "S ", 2))
		g_tex_char.sprt_textures = get_texpath(str + 2, check_settings, 5);
	else if(ft_strnstr(str, "F ", 2))
		g_tex_char.flr_textures = get_backcolor(str + 2, check_settings, 6);
	else if(ft_strnstr(str, "C ", 2))
		g_tex_char.cl_textures = get_backcolor(str + 2, check_settings, 7);
	// check settings
	return (0);
}

char	**ft_read_map(t_all *all, char **argv)
{
	int		fd;
	char	*line;
	t_list	*begin;
	
	int	check_settings[9];
	ft_bzero(check_settings, 9);
			
	line = NULL;
	begin = NULL;
	fd = open(argv[1], O_RDONLY);
	while (get_next_line(fd, &line))
	{
		if (ft_strchr("NRSWEFC", *line))
			ft_parse_settings(all, line, check_settings);
		else // parse map 
			ft_lstadd_back(&begin, ft_lstnew(ft_cut_space(line)));
		free(line);// а не протекает ли?
		line = NULL;
	}
	ft_lstadd_back(&begin, ft_lstnew(ft_cut_space(line)));
	free(line);

	for (size_t i = 0; i < 9; i++)
	{
		printf("{%d} check\n", check_settings[i]);
	}
	
	
	close(fd);
	
	return (make_map(&begin, ft_lstsize(begin)));
}

// int		main()
// {
// 	char **map;

// 	map = ft_read_map();
// 	int	i = 0;
// 	while (map[i])
// 	{
// 		ft_putendl_fd(map[i], 1);
// 		i++;
// 	}
// 	return 0;
// }
