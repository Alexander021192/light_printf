#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void	int_to_char(unsigned char *start, int value)
{
	start[0] = (unsigned char)(value);
	start[1] = (unsigned char)(value >> 8);
	start[2] = (unsigned char)(value >> 16);
	start[3] = (unsigned char)(value >> 24);
}

int	get_bmp_header(int fd, int filesize)
{
	int				i;
	
	unsigned char	bmpfileheader[54];

	i = 0;
	while (i < 54)
		bmpfileheader[i++] = (unsigned char)(0);
	bmpfileheader[0] = (unsigned char)('B');
	bmpfileheader[1] = (unsigned char)('M');
	int_to_char(bmpfileheader + 2, filesize);
	bmpfileheader[10] = (unsigned char)(54);
	bmpfileheader[14] = (unsigned char)(40); //???
	
	int_to_char(bmpfileheader + 18, 64);
	
	int_to_char(bmpfileheader + 22, 64);

	bmpfileheader[27] = (unsigned char)(1);
	bmpfileheader[28] = (unsigned char)(24);
	return (!(write(fd, bmpfileheader, 54) < 0));
}

static int
	write_bmp_data(int file, int pad)
{
	const unsigned char	zero[3] = {0, 0, 0};
	int					i;
	int					j;
	int					color;
	color = 14680288;
	i = 0;
	while (i < 64)
	{
		j = 0;
		while (j < 64)
		{
			color += j;
			if (write(file, &color, 3) < 0)
				return (0);
			if (pad > 0 && write(file, &zero, pad) < 0)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int
	save_bmp(void)
{
	int			filesize;
	int			file;
	int			pad = 0;

	//pad = (4 - (64 * 3) % 4) % 4;
	filesize = 54 + (3 * ((int)64 + pad) * (int)64);
	if ((file = open("screenshot.bmp", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND)) < 0)
		return (0);
	if (!write_bmp_header(file, filesize))
		return (0);
	if (!write_bmp_data(file, pad))
		return (0);
	close(file);
	return (1);
}

int main(int argc, char const *argv[])
{
	save_bmp();
	return 0;
}

