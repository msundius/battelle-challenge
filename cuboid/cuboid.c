// Program: Take in 4 inputs. 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void generate_cuboid(char cuboid[], int x_len, int y_len, int z_len);

void main(int argc, char ** argv)
{
	int x, y, z;
	int debug = 0;

	// Check to make sure user passes three arguments:
	if (argc != 4)
	{
		if ((argc == 5) && (*argv[4] == 'd')) {
			debug = 1;
		}
		else {
			printf("Please pass the following three arguments: x, y, z, %d\n", argc);
			return;
		}
	}	
	x = atoi(argv[1]);
	y = atoi(argv[2]);
	z = atoi(argv[3]);

	/* if any args bad exit with error */
	if (((x <= 0) || (x > 9)) && !debug)
		exit(1);
	if (((y <= 0) || (y > 9)) && !debug)
		exit(1);
	if (((z <= 0) || (z > 9)) && !debug)
		exit(1);

	// Print out and store cuboid dimensions
	printf("Cuboid length: ");
	printf("%d\n", x);

	printf("Cuboid width: ");
	printf("%d\n", y);

	printf("Cuboid height: ");
	printf("%d\n", z);

	if (debug)
		return;

	// Allocate memory to store cuboid shape characters.
	// display is (1 + y + z) * (3*x+y+z)
        // if display is square and spaces after.. so this is safe

	char * cuboid = malloc((1 + y + z) * (3*x+y+z));

	// Generate the cuboid output we can pass to printf to print cuboid.
	generate_cuboid(cuboid, x, y, z);

	printf("%s", cuboid);
	printf("\n");
}

void generate_cuboid(char cuboid[], int x_len, int y_len, int z_len)
{
	int lines = z_len + y_len;
	int index = 0;
	int i, j;
	for (i = 0; i < lines; i++)
	{
		int k = index;

		// Calculate number of spaces for this line
		int spaces = y_len - i;
		spaces = abs(spaces);
		while (spaces > 0)
			cuboid[k++] = ' ';
			spaces--;

		// Insert first edge on each line
		if ((i <= y_len) && i != 0)
		{
			cuboid[k++] = ' ';
			cuboid[k++] = '/';
		}
		else if (i > y_len)
		{
			cuboid[k++] = '\\';
			cuboid[k++] = ' ';
		}
		else
		{
			cuboid[k++] = ' ';
			cuboid[k++] = ' ';
		}

		// Calculate underscores
		int m;
		for (m = 0; m < x_len; m++)
		{
			cuboid[k++] = '_';

			if ((i = 0))
			{
				cuboid[k++] = ' ';
				cuboid[k++] = ' ';
			}
			if ((i <= y_len) && i != 0)
			{
				cuboid[k++] = ' ';
				cuboid[k++] = '/';
			}
			else if (i > y_len)
			{
				cuboid[k++] = '\\';
				if (m != (x_len - 1))
					cuboid[k++] = ' ';
			}
		}

		// Calculate remaining edges
		int side_chars;
		if (i <= z_len) {
			side_chars = i;
		}
		else {
			side_chars = lines - i;
		}
		for (m = 0; m < side_chars; m++) {
			if ((i = 0))
			{
				cuboid[k++] = ' ';
				cuboid[k++] = ' ';
			}
			if (i <= y_len && i != 0)
			{
				if (m < y_len && m < z_len)
				{
					cuboid[k++] = '\\';
					if (m < (side_chars - 1))
						cuboid[k++] = '/';
				}			
			}
			else if (i > y_len)
			{
				if (m < y_len && m < z_len)
				{
					cuboid[k++] = '/';
					if (m < (side_chars - 1))
						cuboid[k++] = '\\';
				}
			}
		}
		index = k;	
	}
}
