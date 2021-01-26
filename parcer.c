/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexandr <alexandr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 12:21:43 by alexandr          #+#    #+#             */
/*   Updated: 2021/01/26 17:04:59 by alexandr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/libft.h"
#include "./includes/cub.h"

#include <stdio.h>

void	ft_lstprint(t_list *lst)
{
	while (lst)
	{
		printf("%s\n", (char *)lst->content);
		lst = lst->next;
	}
}

int		make_map(t_list **begin, int size)
{
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = *begin;
	g_pars.map = ft_calloc(size + 1, sizeof(char*));
	while (tmp)
	{
		g_pars.map[i++] = tmp->content;
		tmp = tmp->next;
	}
	ft_lstclear(begin, free);
	i = 0;
	while (g_pars.map[i])
	{
		ft_putendl_fd(g_pars.map[i], 1);
		i++;
	}
	return (0);
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

int main(int argc, char const *argv[])
{
	int		fd;
	char	*line;
	t_list	*begin;
	t_list	*tmp;
	
	line = 0;
	begin = 0;
	fd = open("./maps/test.cub", O_RDONLY);
	while (get_next_line(fd, &line) > 0)
	{

		if (ft_strchr("NRSWEFC", *line))
			ft_parse_settings(line);
		else // parse map 
		{
			tmp = ft_lstnew(line);
			ft_lstadd_back(&begin, tmp);
			printf("%s\n", (char *)tmp->content);
		}
		free(line);
		line = 0;
	}
	ft_lstadd_back(&begin, ft_lstnew(line));
	//make_map(&begin, ft_lstsize(begin));
	free(line);
	line = 0;

	printf("%d size lst\n", ft_lstsize(begin));
	ft_lstprint(begin);
	// printf("%s\n", g_pars.resolution);
	// printf("%s\n", g_pars.n_textures);
	// printf("%s\n", g_pars.s_textures);
	// printf("%s\n", g_pars.w_textures);
	// printf("%s\n", g_pars.e_textures);
	// printf("%s\n", g_pars.sprt_textures);
	// printf("%s\n", g_pars.flr_textures);
	// printf("%s\n", g_pars.cl_textures);
	
	close(fd);
	return 0;
}
