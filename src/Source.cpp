#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> // abs()
#include <windows.h>
#include <GL/GL.h> // если графика не работает => на весь экран
#include <GL/glut.h>
//#include <gl/GL.h> // в случае чего - расскомментировать
//#include "glut.h" // и закомментировать 2 выше инклюда
#include <string.h>
#include <string>
#define V 0 /* Типы ввода */ 
#define H 1
#define MAX_COUNT_VERTEXES 700
#define UP 2 /* Вид плоскости по нахождению точек отн-о полупространства */
#define DOWN 3
#define NOT_REF 4
int PRECISION = 0;
long double zeroLf = 1;
typedef long double typePlane;
typedef long double typeVert;

typedef struct Planes
{
	int length = 0;
	typePlane arr[4];
	struct Planes *next;
} Planes;
Planes *fPlane1, *lPlane1;
Planes *fPlane2, *lPlane2;
Planes *fPlaneDif, *lPlaneDif;

int adjMatrix[MAX_COUNT_VERTEXES][MAX_COUNT_VERTEXES] = { {0} };
int lengthAdjMat = 1;

int arrVert[MAX_COUNT_VERTEXES] = { 0 }; // Для вывода инфы в плоскостях
int lengthArrVert = 0;

typedef struct Verts
{
	int length = 0;
	typeVert arr[3];
	struct Verts *next;
} Verts;
Verts *fVerts1, *lVerts1;
Verts *fVerts2, *lVerts2;
Verts *fVerts3, *lVerts3;
Verts *fVerts4, *lVerts4;
Verts *fVertsAdj, *lVertsAdj;
Verts *fDif, *lDif;

typePlane maxX, maxY, maxZ, maxMax;
typePlane avX, avY, avZ; // среднее по координатам
float angle = 0.05f; // угол поворота
float dScale = 0.3f; // разность в Масштабе
int rotation = 1;

// Запушить плоскость arr в систему плоскостей fPlane
void pushPlane(typePlane arr[4], Planes **fPlane, Planes **lPlane);

// Запушить точки
void pushVerts(typeVert arr[3], Verts **fVert, Verts **lVert);

// Скопировать из src в trg n элементов
void copyArr(typePlane trg[], typePlane src[], int n);

// Вывести массив arr длины n
void printArr(typeVert arr[], int n);

// Вывести матрицы n * m
void printMatrix(int matrix[][MAX_COUNT_VERTEXES], int n, int m);

// Вывести набор векторов ; mode = 1 выводить с буквами
void printVerts(Verts *verts, int mode = 0);

// Вывети стистему плоскостей
void printPlanes(Planes *fPlane);

// Вывести плоскость
void printPlane(typePlane plane[4]);

// Вывести 2 часть задания про смежные вершины
void printPlanesInf(Planes *fPlane);

int сheckSystemVerts(Verts *fVerts);

// Умножить массив arr длины n на число num
void multArrByNum(typeVert arr[], int n, int num);

// Произвести ввод плоскостей из файла
void enterPlanesFrom(FILE *fl, Planes **fPlane = NULL, Planes **lPlane = NULL);

// Произвести ввод точек из файла/консоли (тип ввода V)
void enterVertsFrom(FILE *fl, Verts **fVerts, Verts **lVerts);

// Получить тип ввода: V/H
int getTypeEnter(FILE *fl = NULL);

// Созать прямую в trg из точки start и end
void getStraight(typeVert trg[3], typeVert start[3], typeVert end[3]);

// Создать плоскость в trg из прямой straight1 и straight2 
// с началом координат от start
void getPlane(typePlane trg[4], typeVert straight1[3], typeVert straight2[3], typeVert start[3]);

// Получить тип плоскости - опорная ли; проверкой
// на наличие всех остальных точек лишь в одном
// полупространстве
int getModePlane(typePlane plane[4], Verts *verts);

// НОД
long int gcd(long int a, long int b);

// Определитель из 3-х плоскостей
typePlane getDeterminant(typePlane plane1[4], typePlane plane2[4], typePlane plane3[4]);

// Алгоритм Крамера для 3 * 3 матрицы
// Также возвращает ответ на вопрос: компланрны ли 
int getCramer(typeVert vert[3], typePlane plane1[4], typePlane plane2[4], typePlane plane3[4]);

// Входит ли в выпуклую оболочку
int hasPoint(typeVert arr[3], Planes *fPlane);

// Корректировка вещественного нуля
void setCorrectZero(typeVert vert[], int n);

// Поиск максимальных x y z и среднее значение по координатам
void setMaxAndAv();

// Проверка на равенство массивов длины n
int isEqualArrs(typeVert arr1[], typeVert arr2[], int n);

// Проверка на уникальность плоскости из списка
int isUniqPlane(typePlane arr[4], Planes *fPlane);

// Проверка на уникальность точки 
// в системе вершин verts
int isUniqVert(typeVert thisVert[3], Verts *verts);

// Являеются ли плоскости линейно зависимыми
int isLinDep(typePlane arr[4], Planes *fPlane);

// Определить - пересекаются ли выпуклые оболочки
int isMinkDif(Verts *vert1, Verts *vert2);

// Сократить коэффициенты в плоскости
int reduce(typePlane arr[4]);

// Создать систему плоскотей из векторов
void createSystem(Verts *verts, Planes **fPlane = NULL, Planes **lPlane = NULL);

// Создать матрицу смежности 
void createAdjMatrix(Verts *verts, Planes *fPlane);

// Найти вершины по системе
void findVerts(Verts **fVerts, Verts **lVerts, Planes *fPlane);

// Найти соседние вершны в плоскости plane
void findNeighbV(Verts *verts, typePlane plane[4], int num);

// Графика
void mouseButton(int button, int state, int x, int y);
void drawText(GLfloat x, GLfloat y, GLfloat z, void *font, const std::string str);
void changeSize(int w, int h);
void display();

int main(int argc, char *argv[])
{
	FILE *input1 = NULL, *input2 = NULL;
	int typeEnter = V;

	printf("Do you want to rotate around the oZ axis? 1\\0: _\b");
	if (!scanf("%d", &rotation))
		while (getchar() != '\n');
	printf("***************GRAPHICS**********************\n");
	printf("\tLeft mouse button - increase size.\n");
	printf("\tRight mouse button - reduce size.\n");
	printf("*********************************************\n");

	switch (argc)
	{
	case 2:
		input1 = fopen(argv[1], "rt");
		break;
	case 3:
		input1 = fopen(argv[1], "rt");
		input2 = fopen(argv[2], "rt");
		break;
	default:
		if (argc > 3)
		{
			printf("\nToo many arguments\n");
			return 1;
		}
	}

	typeEnter = getTypeEnter(input1);
	if (argc <= 2)
	{
		if (typeEnter == V)
		{
			enterVertsFrom(input1, &fVerts1, &lVerts1);
			printVerts(fVerts1);

			createSystem(fVerts1, &fPlane1, &lPlane1);
			printf("\nNumber of faces: %d\n", fPlane1->length);
			printPlanes(fPlane1);
			
			findVerts(&fVertsAdj, &lVertsAdj, fPlane1);
			printf("\nNumber of vertices: %d\n", fVertsAdj->length);
			printVerts(fVertsAdj, 1);

			createAdjMatrix(fVertsAdj, fPlane1);
			printf("\nAdjacency matrix\n");
			printMatrix(adjMatrix, fVertsAdj->length, fVertsAdj->length);
			printPlanesInf(fPlane1);
		}
		else if (typeEnter == H)
		{
			PRECISION = 5;
			zeroLf = 0.00001;
			enterPlanesFrom(input1, &fPlane2, &lPlane2);
			printPlanes(fPlane2);

			findVerts(&fVerts2, &lVerts2, fPlane2);
			printf("\nNumber of vertices: %d\n", fVerts2->length);
			printVerts(fVerts2, 1);

			printf("\nAdjacency matrix\n");
			Verts *cur = fVerts2;
			while (cur)// копируем для удобства
			{
				pushVerts(cur->arr, &fVertsAdj, &lVertsAdj);
				cur = cur->next;
			}
			createAdjMatrix(fVertsAdj, fPlane2);
			printMatrix(adjMatrix, fVertsAdj->length, fVertsAdj->length);
			printf("\n");
			printPlanesInf(fPlane2);
		}
		if (argc > 1)
			fclose(input1);
	}
	else if (argc == 3)
	{
		enterVertsFrom(input1, &fVerts3, &lVerts3);
		getTypeEnter(input2);
		enterVertsFrom(input2, &fVerts4, &lVerts4);
		printf("\nNumber of vertices %s: %d\n", argv[1], fVerts3->length);
		printVerts(fVerts3, 1);
		printf("\nNumber of vertices %s: %d\n", argv[2], fVerts4->length);
		printVerts(fVerts4, 1);

		printf("collision detection: ");
		switch (isMinkDif(fVerts3, fVerts4))
		{
		case 1: printf("detected\n");
			break;
		case 0: printf("not detected\n");
			break;
		}

		fclose(input1);
		fclose(input2);
	}

	if (argc > 2)
		return 666;
	
	glutInit(&argc, argv); // Инициализация GLUT и создание окна
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Ooops");

	glutMouseFunc(mouseButton); // регистрация
	glutDisplayFunc(display); 
	glutReshapeFunc(changeSize);

	glutIdleFunc(display); // новая функция для регистрации

	glutMainLoop();
	return 666;
}

void pushPlane(typePlane arr[4], Planes **fPlane, Planes **lPlane)
{
	Planes *newEl = (Planes*)malloc(sizeof(Planes));

	if (!newEl)
	{
		printf("Couldn't allocate memory");
		return;
	}
	copyArr(newEl->arr, arr, 4);

	if (!(*fPlane))
	{
		(*fPlane) = newEl;
		(*fPlane)->next = NULL;
		(*lPlane) = *fPlane;
		(*fPlane)->length = 1;
	}
	else
	{
		(*lPlane)->next = newEl;
		(*lPlane) = (*lPlane)->next;
		(*lPlane)->next = NULL;
		((*fPlane)->length)++;
	}
}

void pushVerts(typeVert arr[3], Verts **fVert, Verts **lVert)
{
	Verts *newEl = (Verts *)malloc(sizeof(Verts));

	if (!newEl)
	{
		printf("Couldn't allocate memory");
		return;
	}
	copyArr(newEl->arr, arr, 3);

	if (!(*fVert))
	{
		(*fVert) = newEl;
		(*fVert)->next = NULL;
		(*lVert) = *fVert;
		(*fVert)->length = 1;
	}
	else
	{
		(*lVert)->next = newEl;
		(*lVert) = (*lVert)->next;
		(*lVert)->next = NULL;
		((*fVert)->length)++;
	}
}

void copyArr(typePlane trg[], typePlane src[], int n)
{
	for (int i = 0; i < n; i++)
		trg[i] = src[i];
}

void printArr(typeVert arr[], int n)
{
	for (int i = 0; i < n; i++)
		printf("%.*Lf ", PRECISION, arr[i]);
}

void printVerts(Verts *verts, int mode)
{
	int i = 0, num = 0;
	char symbol = 'A';
	Verts *cur = verts;

	while (cur)
	{
		if (mode)
		{
			symbol = i % 26 + 'A';
			num = i / 26;
			if (i < 26)
				printf("%c: ", symbol);
			else if (i >= 26)
				printf("%c%d: ", symbol, num);
		}
		printArr(cur->arr, 3);
		printf("\n");
		cur = cur->next; i++;
	}
}

void printPlanes(Planes *fPlane)
{
	Planes *cur = fPlane;

	while (cur)
	{
		printPlane(cur->arr);
		printf("\n");
		cur = cur->next;
	}
}

void printPlane(typePlane plane[4])
{
	int flagOp = 0;

	if (plane[0])
	{
		if (plane[0] == 1)
			printf("x");
		else if (plane[0] == -1)
			printf("-x");
		else
			printf("%.*Lfx",0, plane[0]); // ТИП ВЫВОДА + ТОЧНОСТЬ
		flagOp = 1;
	}
	if (plane[1])
	{
		if (plane[1] == 1)
			if (flagOp)
				printf("+y");
			else
				printf("y");
		else if (plane[1] == -1)
		{
			printf("-y");
		}
		else if (flagOp)
			printf("%+.*Lfy", 0, plane[1]); // ТИП ВЫВОДА + ТОЧНОСТЬ
		else
			printf("%.*Lfy", 0, plane[1]); // ТИП ВЫВОДА + ТОЧНОСТЬ
		flagOp = 1;
	}
	if (plane[2])
	{
		if (plane[2] == 1)
			if (flagOp)
				printf("+z");
			else
				printf("z");
		else if (plane[2] == -1)
		{
			printf("-z");
		}
		else if (flagOp)
			printf("%+.*Lfz", 0, plane[2]); // ТИП ВЫВОДА + ТОЧНОСТЬ
		else
			printf("%.*Lfz", 0, plane[2]); // ТИП ВЫВОДА + ТОЧНОСТЬ
		flagOp = 1;
	}
	if (flagOp)
		printf("<=%.*Lf", 0, plane[3]); // ТИП ВЫВОДА + ТОЧНОСТЬ
}

void printPlanesInf(Planes * fPlane)
{
	Planes *cur = fPlane;

	while (cur)
	{
		Verts *curVert = fVertsAdj;
		for (int i = 0; i < MAX_COUNT_VERTEXES; i++) // Обнуление
			arrVert[i] = 0;
		lengthArrVert = 0;

		for (int i = 0; curVert; i++)
		{
			if (
				abs((cur->arr[0] * curVert->arr[0] + cur->arr[1] *
					curVert->arr[1] + cur->arr[2] * curVert->arr[2]) - cur->arr[3]) < zeroLf
				)
				arrVert[i] = 1;
			lengthArrVert++;
			curVert = curVert->next;
		}

		printf("Face: ");
		printPlane(cur->arr);
		printf("\n");

		printf("Vertices: ");
		char symbol = 'A';
		int num = 0;

		for (int i = 0; i < lengthArrVert; i++)
		{
			num = i / 26;
			if (arrVert[i])
			{
				symbol = i % 26 + 'A';
				if (i < 26)
					printf("%c, ", symbol);
				else if (i >= 26)
					printf("%c%d, ", symbol, num);
			}
		}
		printf("\b\b  \n");
		printf("Edges: ");
		char symbol1 = 'A', symbol2 = 'B';
		int num1 = 0, num2 = 0;

		for (int i = 0; i <= lengthArrVert; i++)
		{
			symbol1 = i % 26 + 'A';
			num1 = i / 26;
			if (arrVert[i])
			{
				for (int j = i + 1; j < lengthArrVert; j++)
				{
					if (arrVert[j] && adjMatrix[i][j])
					{
						symbol2 = j % 26 + 'A';
						num2 = j / 26;
						if (i < 26 && j < 26)
							printf("%c%c, ", symbol1, symbol2);
						else if (i < 26 && j >= 26)
							printf("%c%c%d, ", symbol1, symbol2, num2);
						else if (i >= 26 && j < 26)
							printf("%c%d%c, ", symbol1, num1, symbol2);
						else if (i >= 26 && j >= 26)
							printf("%c%d%c%d, ", symbol1, num1, symbol2, num2);
					}
				}
			}
		}
		printf("\b\b ");
		printf("\n\n");
		cur = cur->next;
	}
}

void printMatrix(int matrix[][MAX_COUNT_VERTEXES], int n, int m)
{
	int num = 0;
	char symbol = 'A';

	printf("    ");
	for (int i = 0; i < m; i++)
	{
		symbol = i % 26 + 'A';
		num = i / 26;
		if (i < 26)
			printf("%c   ", symbol);
		else if (i >= 26)
			printf("%c%d    ", symbol, num);
	}
	printf("\n");

	for (int i = 0; i < n; i++)
	{
		symbol = i % 26 + 'A';
		num = i / 26;
		if (i < 26)
			printf("%c   ", symbol);
		else  if (i >= 26) 
			printf("%c%d   ", symbol, num);

		for (int j = 0; j < m; j++)
			printf("%d   ", matrix[i][j]);
		printf("\n");
	}
	printf("\n");
}

void multArrByNum(typeVert arr[], int n, int num)
{
	for (int i = 0; i < n; i++)
		arr[i] *= num;
}

void enterPlanesFrom(FILE * fl, Planes **fPlane, Planes **lPlane)
{
	int n = 3, success = 1, length = 0;
	long int inputCoord = 0;

	if (!fscanf(fl, "%d\n", &length))
	{
		printf("\nInvalid enter in file\n");
		exit(1);
	}
	for (int i = 0; i < length; i++)
	{
		typePlane arr[4] = { 0, 0, 0, 0 };
		for (int j = 0; j < 4; j++)
		{
			success = fscanf(fl, "%li", &inputCoord); // ТИП ВВОДА
			if (success != 1)
			{
				printf("\nInvalid enter in file\n");
				exit(1);
			}
			arr[j] = inputCoord;
		}
		pushPlane(arr, fPlane, lPlane);
	}
	(*fPlane)->length = length;
}

void enterVertsFrom(FILE * fl, Verts **fVerts, Verts **lVerts)
{
	long int inputCoord = 1;
	typeVert arr[3] = { 0, 0, 0 };
	int length = 0; int success = 1;

	if (fl)
	{
		if (!fscanf(fl, "%d\n", &length))
		{
			printf("\nInvalid enter in file\n");
			exit(1);
		}
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				success = fscanf(fl, "%li", &inputCoord); // ТИП ВВОДА
				if (!success)
				{
					printf("\nInvalid enter in file\n");
					exit(1);
				}
				arr[j] = inputCoord;
			}
			pushVerts(arr, fVerts, lVerts);
		}
	}
	else
	{
		printf("\nEnter the number of points and the points themselves\n");
		if (!scanf("%d", &length))
		{
			printf("\nInvalid enter\n");
			exit(1);
		}
		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				success = scanf("%li", &inputCoord); // ТИП ВВОДА
				if (success != 1)
				{
					printf("\nInvalid enter\n");
					exit(1);
				}
				arr[j] = inputCoord;
			}
			pushVerts(arr, fVerts, lVerts);
		}
		printf("\n***********************************\n");
	}
}

int getTypeEnter(FILE * fl)
{
	int type = V, success = 1;
	char symbol = '0';

	if (fl)
	{
		fscanf(fl, "%c\n", &symbol);
		switch (symbol)
		{
		case 'H':
		case 'h':
			type = H;
			break;
		case 'V':
		case 'v':
			type = V;
			break;
		default:
			printf("Invalid type of enter in file");
			exit(1);
		}
	}

	return type;
}

void getStraight(typeVert trg[3], typeVert start[3], typeVert end[3])
{
	for (int i = 0; i < 3; i++)
		trg[i] = end[i] - start[i];
	setCorrectZero(trg, 3);
}

void getPlane(typePlane trg[4], typeVert straight1[3], typeVert straight2[3], typeVert start[3])
{
	trg[0] = straight1[1] * straight2[2] - straight1[2] * straight2[1];
	trg[1] = straight1[2] * straight2[0] - straight1[0] * straight2[2];
	trg[2] = straight1[0] * straight2[1] - straight1[1] * straight2[0];
	trg[3] = -(trg[0] * start[0] + trg[1] * start[1] + trg[2] * start[2]);
	setCorrectZero(trg, 4);
}

int getModePlane(typePlane plane[4], Verts *verts)
{
	int mode = 0;
	typePlane data = 0;
	Verts *curVert = verts;

	for (int i = 0; i < verts->length; i++, curVert = curVert->next)
	{
		data = plane[0] * curVert->arr[0] + plane[1] * curVert->arr[1] +
			plane[2] * curVert->arr[2] + plane[3];
		if (abs(data) < zeroLf)
			data = 0;
		if (mode == UP && data < 0)
			return NOT_REF;
		if (mode == DOWN && data > 0)
			return NOT_REF;
		if (data > 0)
			mode = UP;
		if (data < 0)
			mode = DOWN;
	}
	if (mode == 0)
		mode = DOWN;

	return mode;
}

long int gcd(long int a, long int b)
{
	long int t = a;

	if (a < b)
	{
		t = a;
		a = b;
		b = t;
	}
	while (b != 0) {
		t = b;
		b = a % b;
		a = t;
	}

	return a;
}

typePlane getDeterminant(typePlane plane1[4], typePlane plane2[4], typePlane plane3[4])
{
	typePlane res = plane1[0] * plane2[1] * plane3[2];

	res += plane2[0] * plane3[1] * plane1[2];
	res += plane1[1] * plane2[2] * plane3[0];
	res -= plane3[0] * plane2[1] * plane1[2];
	res -= plane3[1] * plane2[2] * plane1[0];
	res -= plane2[0] * plane1[1] * plane3[2];
	if (abs(res) < zeroLf)
		res = 0;
	return typePlane(res);
}

int getCramer(typeVert vert[3], typePlane plane1[4], typePlane plane2[4], typePlane plane3[4])
{
	typePlane det = getDeterminant(plane1, plane2, plane3);

	if (abs(det) < zeroLf)
		return 0;
	typePlane b11[3] = { plane1[3], plane1[1], plane1[2] };
	typePlane b12[3] = { plane2[3], plane2[1], plane2[2] };
	typePlane b13[3] = { plane3[3], plane3[1], plane3[2] };
	typePlane detB = getDeterminant(b11, b12, b13);
	vert[0] = detB / det;
	typePlane b21[3] = { plane1[0], plane1[3], plane1[2] };
	typePlane b22[3] = { plane2[0], plane2[3], plane2[2] };
	typePlane b23[3] = { plane3[0], plane3[3], plane3[2] };
	detB = getDeterminant(b21, b22, b23);
	vert[1] = detB / det;
	typePlane b31[3] = { plane1[0], plane1[1], plane1[3] };
	typePlane b32[3] = { plane2[0], plane2[1], plane2[3] };
	typePlane b33[3] = { plane3[0], plane3[1], plane3[3] };
	detB = getDeterminant(b31, b32, b33);
	vert[2] = detB / det;
	setCorrectZero(vert, 3);

	return 1;
}

void setCorrectZero(typeVert vert[], int n)
{
	for (int i = 0; i < n; i++)
	{
		if (abs(vert[i]) < zeroLf)
			vert[i] = 0.0;
	}
}

int isEqualArrs(typeVert arr1[], typeVert arr2[], int n)
{
	int equal = 1;

	for (int i = 0; i < n; i++)
		if (abs(arr1[i] - arr2[i]) >= zeroLf)
		{
			equal = 0;
			break;
		}

	return equal;
}

int isUniqPlane(typePlane arr[4], Planes * fPlane)
{
	int uniq = 1;
	Planes *cur = fPlane;

	if (!fPlane)
		return uniq;
	while (cur)
	{
		if (isEqualArrs(arr, cur->arr, 4))
		{
			uniq = 0;
			break;
		}
		cur = cur->next;
	}

	return uniq;
}

int isUniqVert(typeVert thisVert[3], Verts *verts)
{
	int uniq = 1;
	Verts *curVert = verts;

	while (curVert)
	{
		if (isEqualArrs(curVert->arr, thisVert, 3))
		{
			uniq = 0;
			break;
		}
		curVert = curVert->next;
	}

	return uniq;
}

int isLinDep(typePlane arr[4], Planes * fPlane)
{
	Planes *cur = fPlane;
	int linDep = 0;
	int gcdPlane = reduce(arr);

	if (!gcdPlane)
		return linDep;
	if (!fPlane)
		return linDep;
	while (cur)
	{
		if (isEqualArrs(arr, cur->arr, 4))
		{
			linDep = 1;
			break;
		}
		cur = cur->next;
	}

	return linDep;
}

int isMinkDif(Verts * vert1, Verts * vert2)
{
	int collision = 0, index = 0;
	Verts *curVert1 = vert1, *curVert2 = vert2;

	for (int i = 0; curVert1; i++)
	{
		curVert2 = vert2;
		for (int j = 0; curVert2; j++)
		{
			typeVert arr[3] = { 0, 0, 0 };
			for (int k = 0; k < 3; k++)
				arr[k] = curVert1->arr[k] - curVert2->arr[k];
			setCorrectZero(arr, 3);

			if (isUniqVert(arr, fDif))
				pushVerts(arr, &fDif, &lDif);

			curVert2 = curVert2->next;
		}

		curVert1 = curVert1->next;
	}

	typeVert zero[3] = { 0, 0, 0 };
	Verts *curVert = fDif;
	while (curVert)
	{
		if (isEqualArrs(curVert->arr, zero, 3))
			return 1;
		curVert = curVert->next;
	}

	createSystem(fDif, &fPlaneDif, &lPlaneDif);
	if (hasPoint(zero, fPlaneDif))
		collision = 1;

	return collision;
}

int hasPoint(typeVert arr[3], Planes *fPlane)
{
	int correct = 1;
	Planes *cur = fPlane;

	while (cur)
	{
		if (
			((cur->arr[0] * arr[0] + cur->arr[1] * arr[1]
			+ cur->arr[2] * arr[2]) - cur->arr[3]) >= zeroLf
			)
		{
			correct = 0;
			break;
		}
		cur = cur->next;
	}

	return correct;
}

int reduce(typePlane plane[4])
{
	long int arr[4];
	long int num = pow(10, PRECISION + 1);
	for (int i = 0; i < 4; i++)
		arr[i] = long int(plane[i] * num);
	long int gcdPlane = gcd(gcd(arr[0], arr[1]), gcd(arr[2], arr[3]));

	gcdPlane = abs(gcdPlane);
	if (!gcdPlane)
		return 0;
	for (int i = 0; i < 4; i++)
	{
		arr[i] /= (gcdPlane);
		plane[i] = arr[i];
	}
	
	return 1;
}

int сheckSystemVerts(Verts * fVerts)
{
	Verts *fVerts1 = fVerts;
	Verts *fVerts2 = fVerts;
	Verts *fVerts3 = fVerts;

	for (int i = 0; i < fVerts->length - 2; i++, fVerts1 = fVerts1->next)
	{
		fVerts2 = fVerts1->next;
		for (int j = i + 1; j < fVerts->length - 1; j++, fVerts2 = fVerts2->next)
		{
			fVerts3 = fVerts2->next;
			for (int k = j + 1; k < fVerts->length; k++, fVerts3 = fVerts3->next)
			{
				if (abs(getDeterminant(fVerts1->arr, fVerts2->arr, fVerts3->arr)) >= zeroLf)
					return 0;
			}
		}
	}
	return 1;

}

void createSystem(Verts * verts, Planes **fPlane, Planes **lPlane)
{
	typeVert straight1[3] = { 0, 0, 0 };
	typeVert straight2[3] = { 0, 0, 0 };
	typePlane curPlane[4] = { 0, 0, 0, 0 };
	Verts *curVert1 = verts, *curVert2 = verts, *curVert3 = verts;

	for (int i = 0; i < verts->length - 2; i++, curVert1 = curVert1->next)
	{
		curVert2 = curVert1->next;
		for (int j = i + 1; j < verts->length - 1; j++, curVert2 = curVert2->next)
		{
			curVert3 = curVert2->next;
			for (int k = j + 1; k < verts->length; k++, curVert3 = curVert3->next)
			{
				getStraight(straight1, curVert1->arr, curVert2->arr);
				getStraight(straight2, curVert1->arr, curVert3->arr);
				getPlane(curPlane, straight1, straight2, curVert1->arr);
				setCorrectZero(curPlane, 4);

				// Точки, лежащие на одной прямой
				if (abs(curPlane[0]) < zeroLf && abs(curPlane[1]) < zeroLf &&
					abs(curPlane[2]) < zeroLf && abs(curPlane[3]) < zeroLf)
					continue;

				int modePlane = getModePlane(curPlane, verts);
				curPlane[3] *= -1;
				if (modePlane == UP)
				{
					multArrByNum(curPlane, 4, -1);
					setCorrectZero(curPlane, 4);
					if (PRECISION)
					{
						if (isUniqPlane(curPlane, *fPlane) /*!isLinDep(curPlane, *fPlane)*/)
							pushPlane(curPlane, fPlane, lPlane);
					}
					else if (isUniqPlane(curPlane, *fPlane) && !isLinDep(curPlane, *fPlane))
						pushPlane(curPlane, fPlane, lPlane);
				}
				else
				{
					setCorrectZero(curPlane, 4);
					if (modePlane == DOWN)
					{
						if (PRECISION)
						{
							if (isUniqPlane(curPlane, *fPlane) /*!isLinDep(curPlane, *fPlane)*/)
								pushPlane(curPlane, fPlane, lPlane);
						}
						else if (isUniqPlane(curPlane, *fPlane) && !isLinDep(curPlane, *fPlane))
							pushPlane(curPlane, fPlane, lPlane);
					}
				}
			}
		}
	}

	if (!(*fPlane))
	{
		printf("\nThe convex hull is empty\n");
		exit(1);
	}
}

void createAdjMatrix(Verts *verts, Planes *fPlane)
{
	Verts *curVert = verts;

	for (int i = 0; curVert; i++, curVert = curVert->next)
	{
		Planes *cur = fPlane;
		while (cur)
		{
			if (abs((cur->arr[0] * curVert->arr[0] + cur->arr[1] * curVert->arr[1]
				+ cur->arr[2] * curVert->arr[2]) - cur->arr[3]) < zeroLf)
				findNeighbV(verts, cur->arr, i);
			cur = cur->next;
		}

		for (int j = 0; j < verts->length; j++)
			if (adjMatrix[i][j] >= 2)
				adjMatrix[i][j] = 1;
			else
				adjMatrix[i][j] = 0;
	}
}

void findVerts(Verts **fVert, Verts **lVert, Planes *fPlane)
{
	Planes *plane1 = fPlane, *plane2 = fPlane, *plane3 = fPlane;
	typeVert vert[3] = { 0, 0, 0 };

	while (plane1)
	{
		plane2 = fPlane;
		while (plane2)
		{
			plane3 = fPlane;
			while (plane3)
			{
				int isNotCoplanar = getCramer(vert, plane1->arr, plane2->arr, plane3->arr);
				setCorrectZero(vert, 3);
				if (
					isNotCoplanar && hasPoint(vert, fPlane) && isUniqVert(vert, *fVert)
					)
					pushVerts(vert, fVert, lVert);

				plane3 = plane3->next;
			}
			plane2 = plane2->next;
		}
		plane1 = plane1->next;
	}

	if (сheckSystemVerts(*fVert) && (*fVert)->length > 3)
	{
		(*fVert)->length = 0;
		(*fVert) = NULL;
		(*lVert) = NULL;
		findVerts(fVert, lVert, fPlane);
	}
	if (!(*fVert))
	{
		printf("\nVertex system is empty\n");
		exit(1);
	}
}

void findNeighbV(Verts *verts, typePlane plane[4], int num)
{
	Verts *curVert = verts;

	for (int i = 0; curVert; i++, curVert = curVert->next)
		if (
			(abs((plane[0] * curVert->arr[0] + plane[1] * curVert->arr[1] +
				plane[2] * curVert->arr[2]) - plane[3])) < zeroLf && (i != num)
			)
		{
			adjMatrix[num][i]++;
			lengthAdjMat++;
		}
}

/* ГРАФИКА */
void mouseButton(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP)
		{
			glMatrixMode(GL_MODELVIEW);
			glScalef(1+dScale, 1 + dScale, 1 + dScale);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			glMatrixMode(GL_MODELVIEW);
			glScalef(1 - dScale, 1 - dScale, 1 - dScale);
		}
	}
}

void setMaxAndAv()
{
	maxX = fVertsAdj->arr[0]; maxY = fVertsAdj->arr[1];	maxZ = fVertsAdj->arr[2];
	Verts *cur = fVertsAdj;
	avX = 0, avY = 0, avZ = 0;

	for (int i = 0; cur; i++, cur = cur->next)
	{
		avX += cur->arr[0];
		avY += cur->arr[1];
		avZ += cur->arr[2];
		maxX = max(abs(maxX), abs(cur->arr[0]));
		maxY = max(abs(maxY), abs(cur->arr[1]));
		maxZ = max(abs(maxZ), abs(cur->arr[2]));
	}
	maxX += 2; maxY += 2; maxZ += 2;

	avX /= fVertsAdj->length;
	avY /= fVertsAdj->length;
	avZ /= fVertsAdj->length;

	maxMax = abs(max(maxX, maxY));
	maxMax = abs(max(maxMax, maxZ));
}

void drawText(GLfloat x, GLfloat y, GLfloat z, void *font, const std::string str)
{
	glRasterPos3d(x, y, z);
	for (int i = 0; str[i] != '\0'; i++)
		glutBitmapCharacter(font, str[i]);
}

void changeSize(int w, int h) {
	if (h == 0) // предотвращение деления на ноль
		h = 1;
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45, ratio, 0.1f, abs(maxMax) +200.0f);
	gluLookAt(maxX, maxY, maxZ, avX, avY, avZ, 0, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	setMaxAndAv();	

	if (rotation)
		glRotatef(angle, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT);
	// Координатные оси
	glColor3d(0.41, 0.41, 1);
	glBegin(GL_LINES);
	glColor3d(0.41, 0.41, 1);
		glVertex3d(0, 0, 0); 	//x
		glVertex3d(maxX, 0, 0);
			 
		glVertex3d(0, 0, 0); 	//y
		glVertex3d(0, maxY, 0);
			 
		glVertex3d(0, 0, 0);    //z
		glVertex3d(0, 0, maxZ);
	glEnd();
	glBegin(GL_LINES);
	glColor3d(0.61, 0.61, 1);
		glVertex3d(0, 0, 0); // -x
		glVertex3d(-maxX, 0, 0);
			 
		glVertex3d(0, 0, 0); // -y
		glVertex3d(0, -maxY, 0);
			 
		glVertex3d(0, 0, 0); // -z
		glVertex3d(0, 0, -maxZ);

		glVertex3d(maxX * 0.9, 0.08 * maxMax, 0); // стрелка по X
		glVertex3d(maxX, 0, 0);
		glVertex3d(maxX * 0.9, -0.08 * maxMax, 0);
		glVertex3d(maxX, 0, 0);

		glVertex3d(0.08 * maxMax, maxY * 0.9, 0); // стрелка по Y
		glVertex3d(0, maxY, 0);
		glVertex3d(-0.08 * maxMax, maxY * 0.9, 0);
		glVertex3d(0, maxY, 0);

		glVertex3d(0.08 * maxMax, 0, maxZ * 0.9); // стрелка по Z
		glVertex3d(0, 0, maxZ);
		glVertex3d(0, 0.08 * maxMax, maxZ * 0.9);
		glVertex3d(0, 0, maxZ);
	glEnd();

	// подпись координатных осей
	glColor3f(1, 0, 0); 
	drawText(maxX, 0.1, 0, GLUT_BITMAP_TIMES_ROMAN_24, "X");
	drawText(0.1, maxY, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Y");
	drawText(0, 0.1, maxZ, GLUT_BITMAP_TIMES_ROMAN_24, "Z");

	char symbol = 'A';
	int sNum = 0;
	Verts *curVert = fVertsAdj;

	for (int i = 0; curVert; i++, curVert = curVert->next)
	{
		symbol = i % 26 + 'A';
		sNum = i / 26;
		char symbols[2] = { symbol, '\0' };
		long int num = (long int)curVert->arr[0];
		std::string strNum = std::to_string(num);
		std::string str = symbols;

		if (i >= 26)
			str += std::to_string(sNum);
		int strLen = strNum.length();
		if (!PRECISION)
			PRECISION--;

		str += "(";
		str += std::to_string(curVert->arr[0]).substr(0, strLen + PRECISION + 1);

		num = (long int)curVert->arr[1];
		strNum = std::to_string(num);
		strLen = strNum.length();
		str += " " + std::to_string(curVert->arr[1]).substr(0, strLen + PRECISION + 1);

		num = (long int)curVert->arr[2];
		strNum = std::to_string(num);
		strLen = strNum.length();
		str += " " + std::to_string(curVert->arr[2]).substr(0, strLen + PRECISION + 1);
		str += ")";

		glColor3f(1.0, 1.0, 1.0);
		drawText(curVert->arr[0], curVert->arr[1], curVert->arr[2],
			GLUT_BITMAP_8_BY_13, str);

		Verts *curVert1 = fVertsAdj;
		for (int j = 0; curVert1; j++, curVert1 = curVert1->next)
		{
			if (adjMatrix[i][j])
			{
				glBegin(GL_LINES);
					glColor3f(0.0, 1.0, 0.0);
					glVertex3d(curVert->arr[0], curVert->arr[1], curVert->arr[2]);
					glVertex3d(curVert1->arr[0], curVert1->arr[1], curVert1->arr[2]);
				glEnd();
			}
		}
	}

	glutSwapBuffers();
}
