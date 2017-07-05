// sdsd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>

template< typename T >
class WrapperMatrix
{
public:
	WrapperMatrix(const int& weight, const int& length);
	void pushLine(const std::vector<T>&&);
	void pushColumn(const std::vector<T>&&);
	void display();
private:
	std::vector<std::vector<T>> matrix;
};
template<typename T>
WrapperMatrix<T>::WrapperMatrix(const int& weight, const int& length) 
{
	this->matrix = std::vector<std::vector<T>>(weight, std::vector<T>(length));
}

template <typename T>
void WrapperMatrix<T>::pushLine(const std::vector<T>&& newLine)
{
	if (newLine.size() == this->matrix.at(0).size())
		matrix.emplace_back(std::move(newLine));
	else
		throw std::invalid_argument("Invalis syntax");
}
template <typename T>
void WrapperMatrix<T>::pushColumn(const std::vector<T>&& newColumn)
{
	if (newColumn.size() == this->matrix.size())
	{
		for (int i = 0; i < matrix.size(); ++i)
			matrix.at(i).emplace_back(std::move(newColumn.at(i)));
	}
	else
		throw std::invalid_argument("Invalid syntax");
}
template<typename T>
void WrapperMatrix<T>::display()
{
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix.at(0).size(); ++j)
			std::cout << matrix.at(i).at(j);
		std::cout << std::endl;
	}
}
int main()
{
	std::vector<int> v1{ 1,2,3,4,5 };
	std::vector<int> v2{ 1,2,3,4,5,6 };
	std::vector<int> v3{ 2,3,4,5,6 };
	
	WrapperMatrix<int> vw(5,5);
	try {
		vw.pushLine(std::move(v1));
		vw.pushColumn(std::move(v2));
		//vw.pushLine(std::move(v3));
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	vw.display();
    return 0;
}

