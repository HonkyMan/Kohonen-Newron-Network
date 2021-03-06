// New_NNKoh.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Math.h"
#include "cstdlib"
#include "ctime"
#include "iostream"

#define STUDY_MAX_ITER 100000
#define APROXIMATION 0.00001
#define TRAINING_SPEED 0.05 
#define TRAINING_SPEED_DELTA 0.001 


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
	int HandleDop(int inputs[]);
	void ShowSum(double *sum);
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
	int Handle(int inputs[], bool flag = 0); // Дает ответ на входные данные (при условии, что сеть обучена)
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

	Newron obj = Newron(28, 10);

	for (int i = 0; i < 10; i++)
	{
		std::cout << i << " = " << obj.Handle(image[i], 1) << std::endl;
	}
	for (int i = 0; i < 10; i++)
	{
		obj.CurrentLearn(image[i], ans[i], STUDY_MAX_ITER, APROXIMATION);
	}
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << " = " << obj.Handle(image[i], 1) << std::endl;
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
			//w[i][j]
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
int Newron::Handle(int inputs[], bool flag)
{
	double *sum = new double[n];
	double *sum1 = new double[n];
	//double sum[10] = {0,0,0,0,0,0,0,0,0,0};
	for (int i = 0; i < m; i++)
	{
		sum[i] = 0;
		sum1[i] = 0;
	}
	//for (int j = 0; j < n; j++)
	//{
	//	w[j][0] = 0;
	//}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//std::cout << newW[i][j] << " ";
			sum[i] += newW[j][i] * inputs[j];
			//if (flag)  std::cout << newW[j][i] << " = " << w[j][i] << std::endl;
			////else sum[i] += w[j][i] * inputs[j];
		}
		//std::cout << std::endl;
		//sum[i] = f(sum[i]);
	}
	if(flag) ShowSum(sum);
	int maxIndex = FindMax(sum);	// Находим максимально значимый 
	//int maxIndex1 = FindMax(sum1);	//int maxIndex1 = FindMax(sum1); // Находим максимально значимый 
	//if (flag) std::cout << maxIndex << " = " << maxIndex1 << std::endl;
	delete[] sum;
	return maxIndex;
}

void Newron::ShowSum(double *sum)
{
	for (size_t i = 0; i < m; i++)
	{
		std::cout << sum[i] << " ";
	}
	std::cout << std::endl;
}

void Newron::Study(int *inputs[], int * answers, int iter, float eps)
{
	for (int i = 0; i < m; i++)
	{
		CurrentLearn(inputs[i], answers[i], iter, eps);
	}
}
inline float TrainingSpeed(int t)
{
	float r;

	r = (float)(TRAINING_SPEED - (t*TRAINING_SPEED_DELTA));
	return (r>TRAINING_SPEED_DELTA) ? r : TRAINING_SPEED_DELTA;
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
				w1[i][CurrentAnswer] += TrainingSpeed(i) * (inputs[i] - w1[i][CurrentAnswer]);
			}
		}
		else
		{
			//std::cout << "Counter: " << counter * TrainingSpeed(counter) * RAND_MAX * rand() << std::endl << "Handle(inputs): " << Handle(inputs) << std::endl << "CurrentAns: " << CurrentAnswer << std::endl << std::endl;
			break;
		}

		counter++;
	} while (iter > counter /*|| MaxNorma(w, w1) > eps*/);
	std::cout << "Counter: " << counter << std::endl << "Handle(inputs): " << HandleDop(inputs) << std::endl << "CurrentAns: " << CurrentAnswer << std::endl << std::endl;
	this->w = w1;
	//Теставая фигня, шоб установить точные веса после обучения/////
	for (int i = 0; i < n; i++)
	{
		newW[i][CurrentAnswer] = w[i][CurrentAnswer];
	}
	std::cout << std::endl;
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
int Newron::HandleDop(int inputs[])
{
	double *sum = new double[n];
	double *sum1 = new double[n];
	for (int i = 0; i < m; i++)
	{
		sum[i] = 0;
		sum1[i] = 0;
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//std::cout << newW[i][j] << " ";
			//sum[i] += newW[j][i] * inputs[j];
			//if (flag)  std::cout << newW[j][i] << " = " << w[j][i] << std::endl;
			sum[i] += w[j][i] * inputs[j];
		}
		//std::cout << std::endl;
		//sum[i] = f(sum[i]);
	}
	int maxIndex = FindMax(sum);	// Находим максимально значимый 
									//int maxIndex1 = FindMax(sum1);	//int maxIndex1 = FindMax(sum1); // Находим максимально значимый 
									//if (flag) std::cout << maxIndex << " = " << maxIndex1 << std::endl;
	delete[] sum;
	return maxIndex;
}