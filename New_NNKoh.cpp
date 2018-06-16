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
	int *x; // входные сигналы
	float *y; // выходные сигналы
	void SetWeights(); //Установка начальных весов
	int FindMax(float *); // Поиск максимального значения
public:
	Newron(int n, int m) : n(n), m(m) //Конструктор
	{
		w = new float *[n];
		for (size_t i = 0; i < n; i++)
		{
			w[i] = new float[m];
		}
		SetWeights();
	}
	~Newron() // Деструктор
	{
		for (size_t i = 0; i < n; i++)
		{
			delete[] w[i];
		}
		delete[]w;
	}
	int Handle(int inputs[]); // Дает ответ на входные данные (при условии, что сеть обучена)
};

inline void Newron::SetWeights()
{
	srand(time(NULL));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			w[i][j] = (RAND_MAX * rand()) % 2 / sqrt(m) - 1 / sqrt(m);
	/*
	srand(time(NULL));
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			w[i][j] = 2.0f / RAND_MAX * rand() - 1.0f;
	*/
}

int Newron::FindMax(float *su)
{
	float max = su[0];
	int maxIndex = 0;
	for (size_t i = 0; i < n; i++)
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
	float *sum = new float[m];
	//float sum[10] = {0,0,0,0,0,0,0,0,0,0};
	for (size_t i = 0; i < m; i++)
	{
		sum[i] = 0;
	}
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			sum[i] += w[j][i] * inputs[j];
		}
	}
	int maxIndex = FindMax(sum); // Находим максимально значимый 
	delete[] sum;
	return maxIndex;
}

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
	int a = obj.Handle(image[0]);
	std::cout << a << std::endl;
	return 0;
}

/*
// Вход 
class Input
{
	// Связи с нейронами
public:
	Link* OutgoingLinks;

};

// Связь входа с нейроном
class Link
{
	// Нейрон
public:
	Neuron Neuron;
	// Вес связи
	double Weight;
};

class Neuron
{
	//Все входы нейрона
public:
	Link* IncomingLinks;
	// Накопленный нейроном заряд 
	double Power;
};
class KohonenNetwork
{
	Input* _inputs;
	int inputLen;
	Neuron* _neurons;
	int newronLen;

	int Handle(int* input);
};



int main()
{
    return 0;
}

int KohonenNetwork::Handle(int * input)
{
	for (int i = 0; i < inputLen; i++)
	{
		auto inputNeuron = _inputs[i];
		for(auto outgoingLink : inputNeuron.OutgoingLinks)
		{
			outgoingLink.Neuron.Power += outgoingLink.Weight * input[i];
		}
	}
	auto maxIndex = 0;
	for (int i = 1; i < newronLen; i++)
	{
		if (_neurons[i].Power > _neurons[maxIndex].Power)
			maxIndex = i;
	}
	//снять импульс со всех нейронов:
	for(auto outputNeuron : _neurons)
	{
		outputNeuron.Power = 0;
	}
	return maxIndex;
}
*/