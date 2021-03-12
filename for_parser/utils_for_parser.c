#include "utils_for_parser.h"
#include <stdio.h>

#define ISDIGIT(x) (x >= '0' && x <= '9')
#define MIN_WIN_WIDTH 640
#define MIN_WIN_HEIGHT 480



size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *src, int c)
{
	size_t i;

	i = 0;
	while (src[i])
	{
		if (src[i] == (char)c)
			return ((char *)src + i);
		i++;
	}
	if (c == '\0')
		return ((char *)(src + i));
	return (0);
}

char	*ft_strrchr(const char *src, int c)
{
	size_t	i;
	char	*p;

	i = 0;
	p = 0;
	while (src[i])
	{
		if (src[i] == (char)c)
			p = (char *)src + i;
		i++;
	}
	if (c == '\0')
		p = (char *)(src + i);
	return (p);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*buf_dest;
	const char	*buf_src;

	if ((dest == src) || n == 0)
		return (dest);
	if (!dest && !src)
		return (0);
	buf_dest = (char *)dest;
	buf_src = (const char *)src;
	while (n--)
		buf_dest[n] = buf_src[n];
	return (dest);
}

char	*ft_strdup(const char *src)
{
	char	*copy;
	size_t	len_src;

	len_src = ft_strlen(src) + 1;
	copy = malloc(len_src);
	if (copy)
		copy = ft_memcpy(copy, src, len_src);
	return (copy);
}

char	*ft_substr(char const *str, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (!str)
		return (0);
	if ((size_t)start > ft_strlen(str))
		return (ft_strdup(""));
	substr = malloc(len + 1);
	i = 0;
	if (substr)
	{
		while (i < len)
			substr[i++] = str[start++];
		substr[i] = 0;
	}
	return (substr);
}

char	*ft_strtrim(char const *str, char const *set)
{
	size_t	start;
	size_t	end;

	if (!str)
		return (0);
	if (!set)
		return (ft_strdup(str));
	start = 0;
	end = ft_strlen(str);
	while (ft_strchr(set, str[start]) && str[start] != '\0')
		start++;
	if (str[start] == '\0')
		return (ft_strdup(""));
	while (ft_strrchr(set, str[end - 1]))
		end--;
	return (ft_substr(str, start, end - start));
}

int		get_resolution(t_all *all, char *str, int* check_settings, int tex_index)
{
	int max_width = 1024;
	int	max_height = 768;
	int	win_width = 0;
	int	win_height = 0;

	//mlx_get_screen_size(all->win.ptr, &max_width, &max_height);
	printf("{%s} {%d}{%d}\n", str, win_width, win_height);
	if(str)
		while(*str == ' ')
			str++;
	while(ISDIGIT(*str))
		win_width = win_width * 10 + (*str++ - '0');

	if(win_width > max_width)
		return(printf("error. Win_width too large\n"));
	else if(win_width < MIN_WIN_WIDTH)
		return(printf("error. Win_width too low\n"));

	while(*str == ' ')
			str++;
	while(ISDIGIT(*str))
		win_height = win_height * 10 + (*str++ - '0');

	if(win_height > max_height)
		return(printf("error. win_height too large\n"));
	else if(win_height < MIN_WIN_HEIGHT)
		return(printf("error. win_height too low\n"));

	check_settings[tex_index] = 1;

	printf("{%s} {%d}{%d}\n", str, win_width, win_height);
	return(0);
	//return(ft_strdup(str));
}

int		get_texpath(t_all *all, char *str, int* check_settings, int tex_index)
{
	char	*tex_path;
	tex_path = ft_strtrim(str, " ");
	if(!tex_path)
		return(printf("error with texture"));
	all->tex[tex_index].path = tex_path;
	check_settings[tex_index] = 1;
	printf("texture {%s}\n", all->tex[tex_index].path);
	return (0);
}


// void 	*get_backcolor(t_all *all, char *str, int* check_settings, int tex_index)
// {
// 	check_settings[tex_index] = 1;
// 	return(ft_strdup(str));
// }

//check settings

int main(void)
{
	t_all	all;
	char	*str = "  640    480     ";
	char	*str1 = "   ./textures/NO.xpm    ";
	int		check_settings[5];
	get_resolution(&all, str, check_settings, 1);
	get_texpath(&all, str1, check_settings, 2);
	return 0;
}
