#include <stdio.h>
#include <stdlib.h>

typedef struct	s_point // структура для точки
{
	double			x;
	double			y;
}				  t_point;

typedef struct	s_sprite
{
	double			dir;
	double			dist;
	double			size;
	t_point			pos;
	struct s_sprite	*next;
}				t_sprite;

int		add_front_sprite(t_sprite **sprites, double dist)
{
	t_sprite	*new;
	t_point		plr_pos;
	
	if (!(new = (t_sprite*)malloc(sizeof(*new))))
		return (0);

	new->dist = dist;
	new->next = *sprites;
	*sprites = new;
	return (1);
}

t_sprite *ft_sort_list(t_sprite *root)
{
	// t_sprite *new_root = root;
	// t_sprite *current = root;
	// t_sprite *next = root->next;
	

	t_sprite *new_root = NULL;
	t_sprite *node = root;
	t_sprite *current = new_root;
	while ( root != NULL )
	{
		node = root;
		root = root->next;

		if ( new_root == NULL || node->dist > new_root->dist )
		{
			node->next = new_root;
			new_root = node;
		}
		else
		{
			current = new_root;
			while ( current->next != NULL && !( node->dist > current->next->dist ) )
			{
				current = current->next;
			}

			node->next = current->next;
			current->next = node;
		}
	}
	return new_root;
}


int main(int argc, char const *argv[])
{
	double i = 5;
	t_sprite *sprites;
	t_sprite *sorted;
	sprites = NULL;
	// while (i > 0)
	// {
	// 	add_front_sprite(&sprites, i);
	// 	i += 0.5 * (-1);
	// }
	i = 0;

	add_front_sprite(&sprites, 9.);
	add_front_sprite(&sprites, 4.);
	add_front_sprite(&sprites, 8.);
	add_front_sprite(&sprites, 2.);

	sorted = sprites;
	printf(" non sorted\n");
	while (sprites)
	{
		printf(" {%.3f}", sprites->dist);
		sprites = sprites->next;
		i += 1; 
	}
	printf("\nsorted\n");
	i = 0;
	sorted = ft_sort_list(sorted);
	while (sorted)
	{
		printf(" {%.3f} ", sorted->dist);
		sorted = sorted->next;
		i += 1; 
	}
	
	return 0;
}
