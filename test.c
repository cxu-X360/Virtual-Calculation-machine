#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

const int SIZE_X = 50;
const int SIZE_Y = 50;
const int scale = 2;
const int rad = scale * SIZE_X / 2;

const double pi = 3.14159262358979;

typedef struct
{
	double x, y, z;
} Vector3;

double x_sphere_half(double y, double z)
{
	return sqrt(rad * rad - y * y - z * z);
}

double dot_mult(Vector3 v1, Vector3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 create_vector (double x1, double y1, double z1, double x2, double y2, double z2)
{
	Vector3 v = {.x = x2 - x1, 
					  .y = y2 - y1, 
					  .z = z2 - z1 };
	return v;
}

Vector3 sum_vector(Vector3 v1, Vector3 v2)
{
	return (Vector3) {v1.x + v2.x,
					  v1.y + v2.y,
					  v1.z + v2.z};
}

Vector3 sub_vector(Vector3 v1, Vector3 v2)
{
	return (Vector3) {v1.x - v2.x,
					  v1.y - v2.y,
					  v1.z - v2.z};
}

Vector3 scale_vector(Vector3 v1, double s)
{
	return (Vector3) {v1.x * s,
					  v1.y * s,
					  v1.z * s};
}

double len_vector(Vector3 v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void print_vector(Vector3 v)
{
	printf("Vector3 = {%lg, %lg, %lg}\n", v.x, v.y, v.z);
}

int calc_shade(double y, double z, Vector3 light)
{
	double x = x_sphere_half(y, z); //Координата х из у и z
	//printf("x = %lg, y = %lg, z = %lg\n", x, y, z);

	Vector3 _n = create_vector(0, x, 0, y, 0, z);
	Vector3 n = scale_vector(_n, 1/len_vector(_n)); //Нормированный вектор нормаоли к пов-сти сферы
	//print_vector(n);

	Vector3 _ray = sub_vector(_n, light);
	Vector3 ray = scale_vector(_ray, 1/len_vector(_ray)); //Нормированный вектор луча из лампы в точку на сфере
	//print_vector(ray);

	double shade_val = (dot_mult(n, ray) + 1) * 7;
	//printf("shade = <%lg>\n", shade_val);

	return (int) shade_val;
}

void draw_sphere(Vector3 light)
{
	//printf("%s started", __FUNCTION__);
	char* shades = "\x25A0@80GCLft1i;:,.";
	char* buffer = calloc(5500, 1);
	int ptr = 0;

	for (int j = 0; j < SIZE_Y; j++)
	{
		for (int i = 0; i < scale * SIZE_X; i++)
		{
			int z = i - rad;
			int y = scale * (j - rad / scale);
			
			if (y * y + z * z < rad * rad)
			{
				buffer[ptr] = shades[calc_shade((double) y, (double) z, light)];
			}
			else
			{
				buffer[ptr] = ' ';
			}
			//putchar(buffer[ptr]);

			ptr++;
		}

		buffer[ptr] = '\n';
		//putchar(buffer[ptr]);
		ptr++;
	}

	fwrite(buffer, 1, 4950, stdout);

	free(buffer);
	//printf("%s ended", __FUNCTION__);
}

int main(int argc, char* argv[])
{	
	double FPS = 55;

	double angle = 0;

	if (argc == 2)
	{
		double FPS = atof(argv[1]);
	}

	while (angle < 4*pi)
	{
		Vector3 light = create_vector(0, 0, 0, 50 * sin (angle) * cos(angle) , 100 * sin(angle), 100 * cos(angle));

		draw_sphere(light);

		angle += 0.1;

		Sleep(1000 / FPS);
	}

	return 0;
}