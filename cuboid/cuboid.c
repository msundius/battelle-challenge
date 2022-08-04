// Program: Take in 4 inputs. 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void generate_cuboid(char cuboid[], int x_len, int y_len, int z_len);

void main(int argc, char ** argv)
{
	// Check to make sure user passes three arguments:
	if (argc != 3)
	{
		printf("Please pass the following three arguments: x, y, z\n");
		return;
	}	
	// Print out and store cuboid dimensions
	printf("Cuboid length: ");
	printf(argv[1]);
	int x = argv[1];
	printf("\n");

	printf("Cuboid width: ");
	printf(argv[2]);
	int y = argv[2];
	printf("\n");

	printf("Cuboid height: ");
	printf(argv[3]);
	int z = argv[3];
	printf("\n");

	// Allocate memory to store cuboid shape characters. Multiple by
	// 2 because that will probably be enough space
	char * cuboid = malloc(x * y * z * 2);

	// Generate the cuboid output we can pass to printf to print cuboid.
	generate_cuboid(&cuboid, x, y, z);

	printf(cuboid);
	printf("\n");
}

void generate_cuboid(char cuboid[], int x_len, int y_len, int z_len)
{
	int lines = y_len + z_len;
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