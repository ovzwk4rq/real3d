#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "mlx.h"

#define BLOCK_SIZE 400
#define MAP_SIZE_BLOCK_X 20
#define MAP_SIZE_BLOCK_Y 20
#define MAP_SIZE_BLOCK_Z 5

#define SCREEN_WIDTH 1800
#define SCREEN_HEIGHT 900
#define SCREEN_DIST 1000

#define SCREEN_LEFT (-SCREEN_WIDTH / 2)
#define SCREEN_TOP (SCREEN_HEIGHT / 2)

#define MY_POS {4000, 4000, 1900}
#define MY_ANGLE {M_PI / 4, -M_PI / 8}

#ifdef MACOS
# define KEY_W 13
# define KEY_S 1
# define KEY_A 0
# define KEY_D 2
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124
#else
# define KEY_W 0x77
# define KEY_S 0x73
# define KEY_A 0x61
# define KEY_D 0x64
# define KEY_UP 0xff52
# define KEY_DOWN 0xff54
# define KEY_LEFT 0xff51
# define KEY_RIGHT 0xff53
#endif

struct s_img
{
	void	*id;
	int		width;
	int		height;
	char	*mem;
	int		bpp;
	int		bypp;
	int		size_line;
	int		endian;
};

struct s_img	texture;

char	map[MAP_SIZE_BLOCK_Z][MAP_SIZE_BLOCK_Y][MAP_SIZE_BLOCK_X] = {
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	},

	{
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},

	{
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},

	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},

	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	},
};

enum e_xyz
{
	X, Y, Z
};

struct s_player
{
	double			pos[3];
	unsigned int	block[3];
	double			vertv_small[3];
	double			vertv_big[3];
	double			angle[2];
	double			matrix[3][3];
};

struct s_raycaster
{
	double		dprop[3];
	double		dprop_delta[3];
	int			block[3];
	int			block_delta[3];
	enum e_xyz	nearest;
};

void	init_raycaster(
		struct s_raycaster *rc,
		struct s_player *me,
		double pixv[3])
{
	enum e_xyz	xyz;

	xyz = X;
	while (xyz <= Z)
	{
		rc->block[xyz] = (int)me->block[xyz];
		if (pixv[xyz] < 0)
		{
			rc->block_delta[xyz] = -1;
			rc->dprop[xyz] = me->vertv_small[xyz] / pixv[xyz];
			rc->dprop_delta[xyz] = -BLOCK_SIZE / pixv[xyz];
		}
		else
		{
			rc->block_delta[xyz] = 1;
			rc->dprop[xyz] = me->vertv_big[xyz] / pixv[xyz];
			rc->dprop_delta[xyz] = BLOCK_SIZE / pixv[xyz];
		}
		xyz++;
	}
}

enum e_xyz	nearest_xyz(double dprop[3])
{
	if (dprop[X] <= dprop[Y] && dprop[X] <= dprop[Z])
		return (X);
	else if (dprop[Y] <= dprop[X] && dprop[Y] <= dprop[Z])
		return (Y);
	else
		return (Z);
}

static inline int	block_out_of_map(int block[3])
{
	return (
			(unsigned int)block[X] >= MAP_SIZE_BLOCK_X ||
			(unsigned int)block[Y] >= MAP_SIZE_BLOCK_Y ||
			(unsigned int)block[Z] >= MAP_SIZE_BLOCK_Z);
}

static inline int	block_has_matter(int block[3])
{
	return (map[block[Z]][block[Y]][block[X]]);
}

void	*ft_memcpy(void *dest, void *src, size_t n)
{
	char	*cdest;
	char	*csrc;

	cdest = dest;
	csrc = src;
	while (n-- > 0)
		*cdest++ = *csrc++;
	return (dest);
}

int	color_of_that_point(double hitp[2])
{
	int	x;
	int	y;
	int	color;

	x = (int)((double)texture.width * hitp[X] / BLOCK_SIZE);
	y = (int)((double)texture.height * hitp[Y] / BLOCK_SIZE);

	if (x < 0)
		x = 0;
	else if (x >= texture.width)
		x = texture.width - 1;

	if (y < 0)
		y = 0;
	else if (y >= texture.height)
		y = texture.height - 1;

	ft_memcpy(
			&color,
			texture.mem + x * texture.bypp + y * texture.size_line,
			sizeof(int));
	return (color);
}

int	color_from_that_block(
		struct s_raycaster *rc,
		struct s_player *me,
		double pixv[3])
{
	double	hitp[2];

	if (rc->nearest == X)
	{
		hitp[X]
			= me->pos[Y] + pixv[Y] * rc->dprop[rc->nearest]
			- (double)(rc->block[Y] * BLOCK_SIZE);
		hitp[Y]
			= me->pos[Z] + pixv[Z] * rc->dprop[rc->nearest]
			- (double)(rc->block[Z] * BLOCK_SIZE);
	}
	else if (rc->nearest == Y)
	{
		hitp[X]
			= me->pos[X] + pixv[X] * rc->dprop[rc->nearest]
			- (double)(rc->block[X] * BLOCK_SIZE);
		hitp[Y]
			= me->pos[Z] + pixv[Z] * rc->dprop[rc->nearest]
			- (double)(rc->block[Z] * BLOCK_SIZE);
	}
	else
	{
		hitp[X]
			= me->pos[X] + pixv[X] * rc->dprop[rc->nearest]
			- (double)(rc->block[X] * BLOCK_SIZE);
		hitp[Y]
			= me->pos[Y] + pixv[Y] * rc->dprop[rc->nearest]
			- (double)(rc->block[Y] * BLOCK_SIZE);
	}
	return (color_of_that_point(hitp));
}

#define SPACE_COLOR 0x00000000

int	color_pointed_by_ray(struct s_player *me, double pixv[3])
{
	struct s_raycaster	rc;

	init_raycaster(&rc, me, pixv);
	while (1)
	{
		rc.nearest = nearest_xyz(rc.dprop);
		rc.block[rc.nearest] += rc.block_delta[rc.nearest];
		if (block_out_of_map(rc.block))
			return (SPACE_COLOR);
		if (block_has_matter(rc.block))
			return (color_from_that_block(&rc, me, pixv));
		rc.dprop[rc.nearest] += rc.dprop_delta[rc.nearest];
	}
}

void	new_image(struct s_img *img, void *mlx, int width, int height)
{
	img->id = mlx_new_image(mlx, width, height);
	img->width = width;
	img->height = height;
	img->mem = mlx_get_data_addr(img->id, &img->bpp, &img->size_line, &img->endian);
	img->bypp = img->bpp / 8;
}

void	draw_square_on_image(struct s_img *img, int bg_color, int bd_color)
{
	int	i;

	i = 0;
	while (i < img->size_line * img->height)
	{
		ft_memcpy(img->mem + i, &bg_color, sizeof(int));
		i += img->bpp / 8;
	}
	i = 0;
	while (i < img->size_line)
	{
		ft_memcpy(img->mem + i, &bd_color, sizeof(int));
		ft_memcpy(
				img->mem + i + (img->height - 1) * img->size_line,
				&bd_color, sizeof(int));
		i += img->bpp / 8;
	}
	i = 0;
	while (i < img->height)
	{
		ft_memcpy(img->mem + i * img->size_line, &bd_color, sizeof(int));
		ft_memcpy(
				img->mem + img->size_line - 4 + i * img->size_line,
				&bd_color, sizeof(int));
		i++;
	}
}

void	new_image_from_xpm(struct s_img *img, void *mlx, char *filename)
{
	img->id = mlx_xpm_file_to_image(mlx, filename, &img->width, &img->height);
	img->mem = mlx_get_data_addr(img->id, &img->bpp, &img->size_line, &img->endian);
	img->bypp = img->bpp / 8;
}

void	rotational_transform(
		double resultv[3],
		double matrix[3][3],
		double opv[3])
{
	resultv[0]
		= matrix[0][0] * opv[0]
		+ matrix[0][1] * opv[1]
		+ matrix[0][2] * opv[2];
	resultv[1]
		= matrix[1][0] * opv[0]
		+ matrix[1][1] * opv[1]
		+ matrix[1][2] * opv[2];
	resultv[2]
		= matrix[2][0] * opv[0]
		+ matrix[2][1] * opv[1]
		+ matrix[2][2] * opv[2];
}

void	get_pixel_vector(
		double pixv[3],
		unsigned int pixel[2],
		struct s_player *me)
{
	double	local_pixv[3];

	local_pixv[X] = SCREEN_LEFT + (double)pixel[X];
	local_pixv[Y] = SCREEN_DIST;
	local_pixv[Z] = SCREEN_TOP - (double)pixel[Y];
	rotational_transform(pixv, me->matrix, local_pixv);
}

void	put_pixel_on_image(struct s_img *img, int color, unsigned int pixel[2])
{
	char	*mem;

	mem	= img->mem
		+ (int)pixel[X] * img->bypp
		+ (int)pixel[Y] * img->size_line;
	ft_memcpy(mem, &color, sizeof(int));
}

void	draw_world_on_image(struct s_img *img, struct s_player *me)
{
	unsigned int	pixel[2];
	double			pixv[3];
	int				color;

	pixel[Y] = 0;
	while (pixel[Y] < SCREEN_HEIGHT)
	{
		pixel[X] = 0;
		while (pixel[X] < SCREEN_WIDTH)
		{
			get_pixel_vector(pixv, pixel, me);
			color = color_pointed_by_ray(me, pixv);
			put_pixel_on_image(img, color, pixel);
			pixel[X]++;
		}
		pixel[Y]++;
	}
}

void	angle_to_matrix(double matrix[3][3], double angle[2])
{
	matrix[0][0] = cos(angle[X]);
	matrix[1][0] = -sin(angle[X]);
	matrix[2][0] = 0;
	matrix[0][1] = cos(angle[Y]) * sin(angle[X]);
	matrix[1][1] = cos(angle[Y]) * cos(angle[X]);
	matrix[2][1] = sin(angle[Y]);
	matrix[0][2] = -sin(angle[Y]) * sin(angle[X]);
	matrix[1][2] = -sin(angle[Y]) * cos(angle[X]);
	matrix[2][2] = cos(angle[Y]);
}

void	update_player(struct s_player *player)
{
	enum e_xyz	xyz;

	xyz = X;
	while (xyz <= Z)
	{
		player->block[xyz] = (unsigned int)floor(player->pos[xyz] / BLOCK_SIZE);
		player->vertv_small[xyz]
			= (double)player->block[xyz] * BLOCK_SIZE - player->pos[xyz];
		player->vertv_big[xyz] = player->vertv_small[xyz] + BLOCK_SIZE;
		xyz++;
	}
	angle_to_matrix(player->matrix, player->angle);
}

void	init_player(struct s_player *player)
{
	double		pos[3] = MY_POS;
	double		angle[2] = MY_ANGLE;

	ft_memcpy(player->pos, &pos, 3 * sizeof(double));
	ft_memcpy(player->angle, &angle, 2 * sizeof(double));
	update_player(player);
}

void	move_player(
		struct s_player *player, double localv[3])
{
	double	velocity[3];

	rotational_transform(velocity, player->matrix, localv);
	player->pos[X] += velocity[X];
	player->pos[Y] += velocity[Y];
	player->pos[Z] += velocity[Z];
	update_player(player);
}

void	rotate_player(struct s_player *player, double rx, double ry)
{
	player->angle[X] += rx;
	player->angle[Y] += ry;
	update_player(player);
}

struct s_draw_param
{
	void			*mlx;
	void			*win;
	struct s_img	*img;
	struct s_player	*me;
	int				key_down;
	int				keycode;
};

int	do_key_down_actions(int keycode, void *param)
{
	struct s_draw_param *dp;
	double				lvleft[3] = {-100, 0, 0};
	double				lvfront[3] = {0, 100, 0};
	double				lvright[3] = {100, 0, 0};
	double				lvback[3] = {0, -100, 0};

	dp = (struct s_draw_param *)param;
	if (keycode == KEY_W) 
		move_player(dp->me, lvfront);
	else if (keycode == KEY_S)
		move_player(dp->me, lvback);
	else if (keycode == KEY_A)
		move_player(dp->me, lvleft);
	else if (keycode == KEY_D)
		move_player(dp->me, lvright);
	else if (keycode == KEY_UP)
		rotate_player(dp->me, 0, M_PI / 64);
	else if (keycode == KEY_DOWN)
		rotate_player(dp->me, 0, -M_PI / 64);
	else if (keycode == KEY_LEFT)
		rotate_player(dp->me, -M_PI / 64, 0);
	else if (keycode == KEY_RIGHT)
		rotate_player(dp->me, M_PI / 64, 0);
	return (0);
}

int	draw(void *param)
{
	struct s_draw_param	*dp;

	dp = (struct s_draw_param *)param;
	if (dp->key_down)
		do_key_down_actions(dp->keycode, dp);
	draw_world_on_image(dp->img, dp->me);
	mlx_put_image_to_window(dp->mlx, dp->win, dp->img->id, 0, 0);
///	rotate_player(dp->me, M_PI / 256, 0);
	return (0);
}

int	key_pressed(int keycode, void *param)
{
	struct s_draw_param	*dp;

	dp = (struct s_draw_param *)param;
	if (!dp->key_down)
	{
		dp->key_down = 1;
		dp->keycode = keycode;
	}
	return (0);
}

int	key_released(int keycode, void *param)
{
	struct s_draw_param *dp;

	dp = (struct s_draw_param *)param;
	if (keycode == dp->keycode)
		dp->key_down = 0;
	return (0);
}

int	main(void)
{
	void			*mlx;
	void			*win;
	struct s_img	img;
	struct s_player	me;
	struct s_draw_param	param;

	mlx = mlx_init();

	win = mlx_new_window(mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "mlx test");
	mlx_clear_window(mlx, win);

	new_image(&texture, mlx, 20, 20);
	draw_square_on_image(&texture, 0x0000ffff, 0x000000ff);
//	new_image_from_xpm(&texture, mlx, "snow_mountain.xpm");

	init_player(&me);

	new_image(&img, mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	param.mlx = mlx;
	param.win = win;
	param.img = &img;
	param.me = &me;
	param.key_down = 0;
//	draw_world_on_image(&img, &me);
//	mlx_put_image_to_window(mlx, win, img.id, 0, 0);

	mlx_hook(win, 2, 0, key_pressed, &param);
	mlx_key_hook(win, key_released, &param);
	mlx_loop_hook(mlx, draw, &param);
	mlx_loop(mlx);
	return (0);
}
