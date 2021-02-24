#include <math.h>
#include <stdio.h>
#include "./includes/mlx.h"

typedef struct s_point
{
	double x;
	double y;
}				t_pos;

typedef	struct s_win
{
	void	*ptr;
	void	*win;
}				t_window;



typedef struct	s_tex
{
	char	*path;
	void	*tex;
	void	*ptr;
	t_pos	start;
	t_pos	end;
	int		width;
	int		height;
	int		bpp;
	int		size_line;
	int		endian;
}				t_tex;

static int
	load_tex(t_window *window, t_tex *tex, char *path)
{
	if (path)
	{
		tex->path = path;
		if ((tex->tex = mlx_xpm_file_to_image(window->ptr,
			path, &tex->width, &tex->height)))
			tex->ptr = mlx_get_data_addr(tex->tex,
				&tex->bpp, &tex->size_line, &tex->endian);
		else
			return (0);
	}
	return (1);
}

int main() 
{
	t_window window;
	t_tex	*tex;
	char	*path = "./maps/wall_1.xpm";

	window.ptr = mlx_init();
	window.win = mlx_new_window(window.ptr, 1000, 1000, "asda");

	tex->tex = mlx_xpm_file_to_image(window.ptr, path, &tex->width, &tex->height);
	tex->ptr = mlx_get_data_addr(tex->tex, &tex->bpp, &tex->size_line, &tex->endian);


	mlx_loop(window.ptr);

}