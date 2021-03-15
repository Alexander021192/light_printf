#include <stdio.h>
#include <string.h>

#define RIGHT 11
#define DOWN 22
#define LEFT 33 
#define UP 44
#define MAP(y,x) map[(int)y][(int)x]

typedef struct s_point
{
	double x;
	double y;
}				t_point;

int		ft_find_plr_pos(char **map, t_point *plr_pos)
{
	int	count_pos = 0;
	t_point pos;

	pos.x = 0;
	pos.y = 0;
	while(map[(int)pos.y])
	{
		while(MAP(pos.y, pos.x))
		{
			if(MAP(pos.y, pos.x) == 'N')
			{

				plr_pos->x = pos.x;
				plr_pos->y = pos.y;
				count_pos++;
			}
			pos.x++;
		}
		pos.x = 0;
		pos.y++;
	}
	return(count_pos);
}

int		map_step(char **map, t_point *pos, int pres_step)
{
	//puts("__________________________\n");
	// printf("{%.f}{%.f} pres pos\n", pos->x, pos->y);
	// printf("{%d}pres step; {%c} char map\n",pres_step, map[(int)pos->y][(int)pos->x]);

	while(1)
	{
		// if((int)pos->y == 9 && (int)pos->x == 9)
		// {
		// 	printf("{%.f}{%.f} pres pos\n", pos->x, pos->y);
		// 	printf("{%d}pres step;\n", pres_step);

		// }
		if(pres_step == RIGHT && (int)pos->y && 
		MAP(pos->y - 1, pos->x) == '1' && pos->y--)
			//printf("{%.f}{%.f} next pos\n", pos->x, pos->y);
			return(UP);
		else if(pres_step == UP && (int)pos->x && 
		MAP(pos->y, pos->x - 1) == '1' && pos->x--)
			//printf("{%.f}{%.f} next pos\n", pos->x, pos->y);
			return(LEFT);
		else if(pres_step == LEFT && map[(int)pos->y + 1] && 
		MAP(pos->y + 1, pos->x) == '1' && ++pos->y)
		//map[(int)pos->y + 1][(int)pos->x] == '1')
		{
			//pos->y++;
			//printf("{%.f}{%.f} next pos\n", pos->x, pos->y);
			return(DOWN);
		}
		else if(pres_step == DOWN && 
		MAP(pos->y, pos->x + 1) == '1' && ++pos->x)
		//map[(int)pos->y][(int)pos->x + 1] == '1')
			//pos->x++;
			//printf("{%.f}{%.f} next pos\n", pos->x, pos->y);
			return(RIGHT);
		pres_step += (pres_step == 44) ? -33 : 11;
	}
//printf("{%.f}{%.f} next pos\n", pos->x, pos->y);
}

int		init_start_pos(char **map, t_point *pos, t_point *start_pos)
{
	start_pos->x = pos->x;
	while((int)pos->y != 0 && (map[(int)pos->y][(int)pos->x] != ' '))
		pos->y--;
	//printf("{%.f}{%c}\n", pos->y, map[(int)pos->y][(int)pos->x]);
	if((int)pos->y == 0 && map[(int)pos->y][(int)pos->x] == '1')
	{
		start_pos->y = pos->y;
		return (1);
	}
	else if(map[(int)pos->y][(int)pos->x] == ' ' && map[(int)pos->y + 1][(int)pos->x] == '1')
	{
		start_pos->y = ++pos->y;
		return(1);
	}
	else
		return(0);
}

int	check_map(char **map)
{
	int i = 0;
	t_point pos;
	t_point start_pos;
	int		pres_step = RIGHT;
	if(ft_find_plr_pos(map, &pos) != 1)
		return(printf("error with position player\n"));
	//printf("{%.f}{%.f} plr pos\n", pos.x, pos.y);
	if(init_start_pos(map, &pos, &start_pos))
	{
		pres_step = map_step(map, &pos, pres_step);
		while ((start_pos.x != pos.x || start_pos.y != pos.y) && ++i)
			pres_step = map_step(map, &pos, pres_step);
		//if(pres_step == RIGHT || pres_step == DOWN)
		//	printf("MAP ok; i = {%d}) {%d} pres = step i\n",i, pres_step);
		//if(pres_step == LEFT || pres_step == UP)
			//return(printf("MAP NOT OK;i = {%d}) {%d} pres = step i\n",i, pres_step));
		// while(pres_step && start_pos.x != pos.x && start_pos.y != pos.y)
		// 	pres_step = map_step(map, &pos, pres_step);
	}
	if(pres_step == LEFT || pres_step == UP || !init_start_pos(map, &pos, &start_pos))
		return(printf("error! not close map"));
	return (0);
}

int	main(void)
{
	char *map[11];

	map[0] = "      111 ";
	map[1] = "     11011";
	map[2] = "     10001";
	map[3] = "1111110001";
	map[4] = "1000000001";
	map[5] = "1000000001";
	map[6] = "1110N01111";
	map[7] = "1110001   ";
	map[8] = "1000001111";
	map[9] = "1111111111";
	map[10] = 0;

	if(check_map(map))
		printf("\nexit game \n");
}