#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <iostream>

GLfloat angle = 0.0f;
GLfloat speed = 5.0f;

GLfloat r = 1.0f;
GLfloat g = 0.0f;
GLfloat b = 0.0f;

#define PI 3.14159265358979323846264338327950288419716939937510

unsigned int points = 3;
unsigned int rotationSpeed = 25;
unsigned int colorChangeFrequency = 2000;
unsigned int updateFrequency = 10;

static int window;

struct vector
{
	float data[3];

	vector()
	{
		for (size_t i = 0; i < 3; i++)
		{
			data[i] = 0;
		}
	}

	vector(float x, float y, float z)
	{
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}

	void print()
	{
		for (size_t y = 0; y < 3; y++)
		{
			std::cout << data[y];
			if (y < 2) std::cout << ", ";

			std::cout << std::endl;
		}
	}
};

template <typename T>
struct matrix3x3
{
	T data[3][3];

	matrix3x3()
	{
		for (size_t y = 0; y < 3; y++)
		{
			for (size_t x = 0; x < 3; x++)
			{
				data[y][x] = 0;
			}
		}
	}

	matrix3x3(T p1, T p2, T p3, T p4, T p5, T p6, T p7, T p8, T p9)
	{
		data[0][0] = p1;
		data[0][1] = p2;
		data[0][2] = p3;

		data[1][0] = p4;
		data[1][1] = p5;
		data[1][2] = p6;

		data[2][0] = p7;
		data[2][1] = p8;
		data[2][2] = p9;
	}

	void print()
	{
		for (size_t y = 0; y < 3; y++)
		{
			for (size_t x = 0; x < 3; x++)
			{
				std::cout << data[y][x];
			}

			std::cout << std::endl;
		}
	}

	matrix3x3 operator*(const matrix3x3& other)
	{
		matrix3x3 newMatrix;

		for (size_t z = 0; z < 3; z++)
		{
			for (size_t y = 0; y < 3; y++)
			{
				newMatrix.data[y][z] = 0;

				for (size_t x = 0; x < 3; x++)
				{
					newMatrix.data[y][z] += data[y][x] * other.data[x][z];
				}
			}
		}

		return newMatrix;
	}

	const vector operator*(const vector other)
	{
		vector newVector;

		for (size_t y = 0; y < 3; y++)
		{
			newVector.data[y] = 0;

			for (size_t x = 0; x < 3; x++)
			{
				newVector.data[y] += data[y][x] * other.data[x];
			}
		}

		return newVector;
	}
};

void renderScene(void)
{
	if (points >= 3 && points <= 8)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glRotatef(angle, 0, 0, 1);

		glBegin(GL_TRIANGLES);

		GLfloat temp = (2.0f * PI) / points;

		for (size_t i = 0; i < points; i++)
		{
			glColor3f(r, g, b);
			glVertex3f(0.0f, 0.0f, 0.0f);

			glColor3f(r, g, b);
			glVertex3f(cos(i * temp), sin(i * temp), 0.0f);

			glColor3f(r, g, b);
			glVertex3f(cos((i + 1) * temp), sin((i + 1) * temp), 0.0f);
		}

		glEnd();

		glutSwapBuffers();
	}
}

void timerCallback(int)
{
	angle += speed;
	glutTimerFunc(rotationSpeed, timerCallback, 0);

}

void colorCallback(int)
{
	r = (rand() % 100) / 100.0f;
	g = (rand() % 100) / 100.0f;
	b = (rand() % 100) / 100.0f;

	glutTimerFunc(colorChangeFrequency, colorCallback, 0);
}

void updateCallback(int)
{
	renderScene();

	glutTimerFunc(updateFrequency, updateCallback, 0);
}

void kbCallback(unsigned char key, int, int)
{
	speed = -speed;
}

void menuCallback(int entry)
{
	if (entry == 1)
	{
		glutDestroyWindow(window);
		exit(0);
	}
	else points = entry;
}

int main(int argc, char** argv)
{
	matrix3x3<float> matrix(1.0f, 2.0f, 3.0f, 2.0f, 4.0f, 0.0f, 5.0f, -1.0f, 2.0f);
	matrix3x3<float> matrix2(1.0f, 2.0f, 0.0f, 4.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	vector vector1(1, 4, 1);

	std::cout << "matrix 1" << std::endl;
	matrix.print();

	std::cout << std::endl << "+ matrix 2" << std::endl;
	matrix2.print();
	std::cout << std::endl << "= matrix 3" << std::endl;

	matrix3x3<float> matrix3 = matrix * matrix2;
	matrix3.print();

	std::cout << std::endl << "vector yolo" << std::endl;

	vector vector2 = matrix * vector1;
	vector2.print();

	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	window = glutCreateWindow("WOOOT");

	glutDisplayFunc(renderScene);
	glutTimerFunc(rotationSpeed, timerCallback, 0);
	glutTimerFunc(colorChangeFrequency, colorCallback, 0);
	glutTimerFunc(updateFrequency, updateCallback, 0);
	glutKeyboardFunc(kbCallback);

	glutCreateMenu(menuCallback);
	glutAddMenuEntry("EXIT", 1);
	glutAddMenuEntry("3", 3);
	glutAddMenuEntry("4", 4);
	glutAddMenuEntry("5", 5);
	glutAddMenuEntry("6", 6);
	glutAddMenuEntry("7", 7);
	glutAddMenuEntry("8", 8);

	glutAttachMenu(GLUT_LEFT_BUTTON);

	glutMainLoop();

	return 1;
}