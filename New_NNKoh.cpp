// New_NNKoh.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Math.h"
#include "cstdlib"
#include "ctime"
#include "iostream"


class Newron
{
	int n; // количество входных сигналов
	int m; // кол-во нейронов скрытого слоя (кол-во определеяемых объектов)
	float **w; // веса входных сигналов (каждый х каждому нейрону)
	float newW[28][10];
	int *x; // входные сигналы
	float *y; // выходные сигналы
	void SetWeights(); //Установка начальных весов
	int FindMax(double *); // Поиск максимального значения
	double f(double s);
	float MaxNorma(float ** w, float ** w1);
public:
	Newron(int n, int m) : n(n), m(m) //Конструктор
	{
		w = new float *[n];
		for (int i = 0; i < n; i++)
		{
			w[i] = new float[m];
		}
		SetWeights();
	}
	~Newron() // Деструктор
	{
		for (int i = 0; i < n; i++)
		{
			delete[] w[i];
		}
		delete[]w;
	}
	int Handle(int inputs[]); // Дает ответ на входные данные (при условии, что сеть обучена)
	void Study(int **inputs, int *answers, int iter, float eps = 0.00001);
	void CurrentLearn(int * inputs, int CurrentAnswer, int iter, float eps);
};


int main()
{
	int image[10][28] = 
	{
		{ 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,1,1,1 },//0
		{ 0,0,0,1, 0,0,1,1, 0,1,0,1, 1,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 },//1
		{ 1,1,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,1,0, 0,1,0,0, 1,1,1,1 },//2
		{ 1,1,1,1, 0,0,1,0, 0,1,0,0, 1,1,1,1, 0,0,1,0, 0,1,0,0, 1,0,0,0 },//3
		{ 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,0,0,1 },//4
		{ 1,1,1,1, 1,0,0,0, 1,0,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1, 1,1,1,1 },//5
		{ 1,1,1,1, 1,0,0,0, 1,0,0,0, 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1 },//6
		{ 1,1,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 },//7
		{ 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1 },//8
		{ 1,1,1,1, 1,0,0,1, 1,0,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,1, 1,1,1,1 } //9
	};
	int ans[10] = { 0,1,2,3,4,5,6,7,8,9 };
	float cipher[10][10] = 
	{
		{ 1,0,0,0,0,0,0,0,0,0 },
		{ 0,1,0,0,0,0,0,0,0,0 },
		{ 0,0,1,0,0,0,0,0,0,0 },
		{ 0,0,0,1,0,0,0,0,0,0 },
		{ 0,0,0,0,1,0,0,0,0,0 },
		{ 0,0,0,0,0,1,0,0,0,0 },
		{ 0,0,0,0,0,0,1,0,0,0 },
		{ 0,0,0,0,0,0,0,1,0,0 },
		{ 0,0,0,0,0,0,0,0,1,0 },
		{ 0,0,0,0,0,0,0,0,0,1 } 
	};

	Newron obj = Newron(28, 10);

	for (int i = 0; i < 10; i++)
	{
		std::cout << i << " = " << obj.Handle(image[i]) << std::endl;
	}
	for (int i = 0; i < 10; i++)
	{
		obj.CurrentLearn(image[i], ans[i], 1000000, 0.00001);
	}
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << " = " << obj.Handle(image[i]) << std::endl;
	}
	//int a = obj.Handle(image[3]);
	//printf("%d", (double)a);
	//std::cout << a << std::endl; 
	return 0;
}
inline void Newron::SetWeights()
{
	/*srand(time(NULL));
	for (int i = 0; i < m; i++)
	for (int j = 0; j < n; j++)
	w[i][j] = (RAND_MAX * rand()) % 2 / sqrt(m) - 1 / sqrt(m);*/

	srand((unsigned int)time(NULL));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			w[i][j] = 2.0f / RAND_MAX * rand() - 1.0f;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			newW[i][j] = w[i][j];
			//std::cout << "w[" << i << "][" << j << "] = " << w[i][j] << "	";
		}
		//std::cout << std::endl;
	}

}

inline int Newron::FindMax(double *su)
{
	double max = su[0];
	int maxIndex = 0;
	for (int i = 1; i < m; i++)
	{
		if (su[i] > max)
		{
			max = su[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}
int Newron::Handle(int inputs[])
{
	double *sum = new double[n];
	//double *sum1 = new double[n];
	//double sum[10] = {0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < m; i++)
	{
		sum[i] = 0;
		//sum1[i] = 0;
	}
	//for (int j = 0; j < n; j++)
	//{
	//	w[j][0] = 0;
	//}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			sum[i] += newW[j][i] * inputs[j];
			//sum1[i] += w[j][i] * inputs[j];
		}
		//sum[i] = f(sum[i]);
	}
	int maxIndex = FindMax(sum); // Находим максимально значимый 
								 //int maxIndex1 = FindMax(sum1); // Находим максимально значимый 
	delete[] sum;
	return maxIndex;
}

void Newron::Study(int *inputs[], int * answers, int iter, float eps)
{
	for (int i = 0; i < m; i++)
	{
		CurrentLearn(inputs[i], answers[i], iter, eps);
	}
}

void Newron::CurrentLearn(int *inputs, int CurrentAnswer, int iter, float eps)
{
	double sum(.0);
	float **w1 = new float*[n];
	for (int i = 0; i < n; i++)
	{
		w1[i] = new float[m];
	}
	w1 = this->w;
	int counter(0);
	do
	{
		if (Handle(inputs) != CurrentAnswer)
		{
			for (int i = 0; i < n; i++)
			{
				w1[i][CurrentAnswer] += 0.5f * (inputs[i] - w1[i][CurrentAnswer]);
			}
		}
		else
		{
			std::cout << "Counter: " << counter << std::endl << "Handle(inputs): " << Handle(inputs) << std::endl << "CurrentAns: " << CurrentAnswer << std::endl << std::endl;
			break;
		}

		counter++;
	} while (iter > counter /*|| MaxNorma(w, w1) > eps*/);
	this->w = w1;
	///Теставая фигня, шоб установить точные веса после обучения/////
	for (int i = 0; i < n; i++)
	{
		newW[i][CurrentAnswer] = w[i][CurrentAnswer];
	}
	////////////////////////
}
inline double Newron::f(double s)
{
	double al = 1.0;
	return 1 / (1 + exp(-al * s));
}
float Newron::MaxNorma(float **w, float **w1)
{
	float s = 0.0f;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
		{
			float r = fabsf(w[i][j] - w1[i][j]);
			if (r > s) s = r;
		}
	return s;

}