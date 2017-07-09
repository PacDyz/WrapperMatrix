// sdsd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>
#include <type_traits>

//template<typename> struct is_std_vector : std::false_type {};
//template<typename T, typename A> struct is_std_vector<std::vector<T,A>> : std::true_type {};
template <typename T, typename U>
struct decay_equiv :
	std::is_same<typename std::decay<T>::type, U>::type
{};


template< typename T >
class WrapperMatrix
{
public:
	WrapperMatrix(const int& weight, const int& length);
	template<typename U>
	std::enable_if_t<decay_equiv<U, std::vector<T>>::value> pushLine(const U&&);
//	template<typename U>
//	std::enable_if_t<is_std_vector<std::decay_t<U>>::value> pushColumn(const U&&);
	template<typename U>
	std::enable_if_t<decay_equiv<U, std::vector<T>>::value> pushColumn(const U&&);
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>> operator+(const U&&);
	void fillMatrix(const T&&);
	void display();
private:
	std::vector<std::vector<T>> matrix;
};
template<typename T>
WrapperMatrix<T>::WrapperMatrix(const int& weight, const int& length) 
{
	this->matrix = std::vector<std::vector<T>>(weight, std::vector<T>(length));
}
template<typename T>
template <typename U>
std::enable_if_t<decay_equiv<U, std::vector<T>>::value>
WrapperMatrix<T>::pushLine(const U&& newLine)
{
	if (newLine.size() == this->matrix.at(0).size())
		matrix.emplace_back(std::move(newLine));
	else
		throw std::invalid_argument("Invalid syntax");
}
template<typename T>
template<typename U>
std::enable_if_t<decay_equiv<U, std::vector<T>>::value>
WrapperMatrix<T>::pushColumn(const U&& newColumn)
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
template<typename T>
template <typename U>
std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>>
WrapperMatrix<T>::operator+(const U&& matrix1)
{
	if(matrix.size() != matrix1.matrix.size() || matrix.at(0).size() != matrix1.matrix.at(0).size())
		throw std::invalid_argument("Invalid syntax");
	for (int i = 0; i < matrix1.matrix.size(); ++i)
	{
		for (int j = 0; j < matrix1.matrix.at(0).size(); ++j)
		{
			matrix.at(i).at(j) += matrix1.matrix.at(i).at(j);
		}
	}
	return *this;
}
template<typename T>
void WrapperMatrix<T>::fillMatrix(const T&& number)
{
	for (int i = 0; i < matrix.size(); ++i)
		std::fill(matrix.at(i).begin(), matrix.at(i).end(), number);
}

int main()
{
	std::vector<int> v1{ 1,2,3,4,5 };
	std::vector<int> v2{ 1,2,3,4,5,6 };
	std::vector<int> v3{ 2,3,4,5,6 };
	std::vector<double> v4{ 1,2,3,4,5,6 };
	WrapperMatrix<int> vw1(5,5);
	WrapperMatrix<int> vw2(6, 6);
	vw2.fillMatrix(1);
	WrapperMatrix<int> vw3(0,0);
	WrapperMatrix<int> vw4(6, 6);
	try {
		vw1.pushLine(std::move(v1));
		vw1.pushColumn(std::move(v2));
		vw3 = vw4 + std::move(vw2);
		//vw.pushLine(std::move(v3));
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	vw1.display();
	vw3.display();
    return 0;
}

