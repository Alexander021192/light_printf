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

int		ft_get_num(char **str)
{
	int	num = 0;
	if(*str)
		while(!ISDIGIT(**str))
			(*str)++;
	while(ISDIGIT(**str))
	{
		num = num * 10 + (**str - '0');
		(*str)++;
	}
	return num;
}

int		get_resolution(t_all *all, char *str, int* check_settings, int tex_index)
{
	int max_width = 1024;
	int	max_height = 768;
	int	win_width = 0;
	int	win_height = 0;

	//mlx_get_screen_size(all->win.ptr, &max_width, &max_height);
	// printf("{%s} {%d}{%d}\n", str, win_width, win_height);

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
	//return(ft_strdup(str));
}

int		get_texpath(t_all *all, char *str, int* check_settings, int tex_index)
{
	char	*tex_path;
	tex_path = ft_strtrim(str, " ");
	if(!tex_path)
		return(printf("error with texture path"));
	all->tex[tex_index].path = tex_path;
	check_settings[tex_index] = 1;
	
	return (0);
}

int		create_color(int b, int g, int r) //поменяли местями красный и синий
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
	//printf("{%s}\n", str);

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
	//return(ft_strdup(str));
}

//check settings

int main(void)
{
	t_all	all;
	char	*str = "  640    480     ";
	char	*str1 = "   ./textures/NO.xpm    ";
	char	*str2 = " 100 , 200, 150";
	char	*str3 = " 0 , 255, 150";
	int		check_settings[8];
	
	get_resolution(&all, str, check_settings, 0);
	for(int i = 1; i < 6; i++)
		get_texpath(&all, str1, check_settings, i);
	get_backcolor(&all, str2, check_settings, 6);
	get_backcolor(&all, str3, check_settings, 7);

	printf("all win {%d}{%d}\n",all.win_width, all.win_height);
	printf("texture {%s}\n", all.tex[2].path);
	printf("back color {%d}{%d}\n", all.ceil_color, all.floor_color);

	int i = -1;
	while(i++ < 7)
	{
		if(!check_settings[i])
			printf("(%d){%d} !\nexit game\n", i, check_settings[i]);
	}
	return 0;
}
