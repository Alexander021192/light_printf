/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 12:21:43 by alexandr          #+#    #+#             */
/*   Updated: 2021/01/27 17:41:37 by ocalamar         ###   ########.fr       */
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
		if(str[i] == ' ')
			count_space++;
	}
	line_without_space = malloc(ft_strlen(str) - count_space);
	i = 0;
	while(str && *str)
	{
		if( *str != ' ')
			line_without_space[i++] = *str;
		str++;
	}
	//free(line) ? 
	
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
	//ft_lstclear(begin, free);
	// i = 0;
	// while (map[i])
	// {
	// 	ft_putendl_fd(map[i], 1);
	// 	i++;
	// }
	return (map);
}

int		ft_parse_settings(char *str)
{
	if(ft_strnstr(str, "R ", 2))
		g_pars.resolution = ft_strdup(str + 2);
	else if(ft_strnstr(str, "NO ", 3))
		g_pars.n_textures = ft_strdup(str + 3);
	else if(ft_strnstr(str, "SO ", 3))
		g_pars.s_textures = ft_strdup(str + 3);
	else if(ft_strnstr(str, "WE ", 3))
		g_pars.w_textures = ft_strdup(str + 3);
	else if(ft_strnstr(str, "EA ", 3))
		g_pars.e_textures = ft_strdup(str + 3);
	else if(ft_strnstr(str, "S ", 2))
		g_pars.sprt_textures = ft_strdup(str + 2);
	else if(ft_strnstr(str, "F ", 2))
		g_pars.flr_textures = ft_strdup(str + 2);
	else if(ft_strnstr(str, "C ", 2))
		g_pars.cl_textures = ft_strdup(str + 2);
	return (0);
}

char	**ft_read_map(void)
{
	int		fd;
	char	*line;
	t_list	*begin;
		
	line = NULL;
	begin = NULL;
	fd = open("./maps/test5.cub", O_RDONLY);
	while (get_next_line(fd, &line))
	{

		if (ft_strchr("NRSWEFC", *line))
			ft_parse_settings(line);
		else // parse map 
			ft_lstadd_back(&begin, ft_lstnew(ft_cut_space(line)));
		// free(line); а не протекает ли?
		// line = NULL;
	}
	ft_lstadd_back(&begin, ft_lstnew(ft_cut_space(line)));
	
	// printf("%d size lst\n", ft_lstsize(begin));
	//ft_lstprint(begin);
	// printf("%s\n", g_pars.resolution);
	// printf("%s\n", g_pars.n_textures);
	// printf("%s\n", g_pars.s_textures);
	// printf("%s\n", g_pars.w_textures);
	// printf("%s\n", g_pars.e_textures);
	// printf("%s\n", g_pars.sprt_textures);
	// printf("%s\n", g_pars.flr_textures);
	// printf("%s\n", g_pars.cl_textures);
	
	close(fd);
	
	return (make_map(&begin, ft_lstsize(begin)));
}
