#define MAP_ERROR 	"map error\n"
#define M_MAX(a,b)	(a>b?a:b)
#define M_MIN(a,b)	(a<b?a:b)

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_list
{
	char ch;
	struct s_list *next;
} ch_list;

typedef struct s_map
{
	char **m_map;
	char obstacle_ch;
	char full_ch;
	char empty_ch;
	int row_num;
} t_map;

typedef struct s_fill
{
	int size;
	int row;
	int col;
} t_fill;

int		ft_atoi(char *s)
{
	int res;
	
	res = 0;
	while (*s >= '0' && *s <= '9')
	{
		res = res * 10 + *s - '0';
		s++;
	}
	return (res);
}

int		ft_strlen(char *s)
{
	int l;
	
	l = 0;
	while (s[l++])
		;
	return (l);
}

int		get_map_param(char *str, t_map *map)
{
	// принимает первую строку файла и возввращает число строку карты
	// или -1 если ошибка
	// записывает в переменные empty/obstacle/full_ch
	int res;
	res = ft_atoi(str);
	if (ft_strlen(str) >= (res / 10 + 3))
	{
		map->empty_ch = str[res / 10 + 1];
		map->obstacle_ch = str[res / 10 + 2];
		map->full_ch = str[res / 10 + 3];
	}
	return (-1);
}

void	print_str(char *str)
{
	while (*str)
	{
		write(1, str, 1);
		str++;
	}
}

char	*read_str(int fd)
{
	ch_list *chain, *first;
	char *s;
	int len, i;
	
	s = NULL;
	i = len = 0;
	first = chain = (ch_list *)malloc(sizeof(ch_list));
	chain->ch = '\0';
	while ((read(fd, &(chain->ch), sizeof(chain->ch)) > 0) && (chain->ch == '\n'))
	{
		len++;
		chain->next = (ch_list *)malloc(sizeof(ch_list));
		chain = chain->next;
	}
	if (len)
	{
		chain = first;
		s = (char *)malloc((len + 1)*sizeof(char));
		while (len--)
		{
			s[i++]=chain->ch;
			chain = chain->next;
			free(first);
			first = chain;
		}
		s[i] = '\0';
	}
	return (s);
}

char	**create_map(int n)
{
	char **ss;
	
	ss = (char **)malloc(n*sizeof(char *));
	return (ss);
}

int		cols_obstacle(t_map map, int row, int col)
{
	int i;
	int j;
	int res;
	
	res = 0;
	i = col;
	while (i < ft_strlen(map.m_map[0]))
	{
		j = row;
		while (j < map.row_num)
		{
			if (map.m_map[j++][i] == map.obstacle_ch)
				return (res);
		}
		res++;
		i++;
	}
	return (res);
}

int		rows_obstacle(t_map map, int row, int col)
{
	int i;
	int j;
	int res;
	
	res = 0;
	i = row;
	while (i < map.row_num)
	{
		j = col;
		while (j < ft_strlen(map.m_map[0]))
		{
			if (map.m_map[i][j++] == map.obstacle_ch)
				return (res);
		}
		res++;
		i++;
	}
	return (res);
}

void	solve_map(t_map map)
{
	/* поиск квадрата*/
	int row;
	int col;
	int cur_size;
	t_fill filler;
	
	row = 0;
	filler.col = 0;
	filler.row = 0;
	filler.size = 0;
	while (row < map.row_num)
	{
		col = 0;
		while (col < ft_strlen(map.m_map[row]))
		{
			if (map.m_map[row][col] != map.obstacle_ch)
			{
				cur_size = M_MIN(cols_obstacle(map,row,col),rows_obstacle(map,row,col));
				if (cur_size>filler.size)
				{
					filler.size = cur_size;
					filler.row = row;
					filler.col = col;
				}
			}
			col++;
		}
		row++;
	}
	//	fill_map(map, filler);
	//	print_map(map);
}

void	process_file(char *file_name)
{
	int fd;
	int cur_line;
	char *line;
	t_map map;
	
	fd = open(file_name, O_RDONLY);
	if (fd != -1)
	{
		line = read_str(fd);
		if ((map.row_num = get_map_param(line, &map) > 0))
		{
			free(line);
			map.m_map = create_map(map.row_num);
			cur_line = 0;
			while ((line = read_str(fd)) != NULL)
				map.m_map[cur_line++] = line;
			solve_map(map);
		}
		else
		{
			print_str(MAP_ERROR);
			free(line);
		}
	}
	else
		print_str(MAP_ERROR);
}

//validate map func

int		main(int argc, char **argv)
{
	while (argc > 0)
	{
		process_file(argv[argc--]);
	}
	return (0);
}
