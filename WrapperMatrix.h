#pragma once
#ifndef FriendWrapperMatrixTest
#define FriendWrapperMatrixTest
#endif 
#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>
#include <type_traits>
#include <utility>
#include <gtest/gtest_prod.h>
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
	std::enable_if_t<decay_equiv<U, std::vector<T>>::value> pushLine(U&&);
	//	template<typename U>
	//	std::enable_if_t<is_std_vector<std::decay_t<U>>::value> pushColumn(const U&&);
	template<typename U>
	std::enable_if_t<decay_equiv<U, std::vector<T>>::value> pushColumn(U&&);
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>> operator+(U&&);
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>> operator-(U&&);
	void fillMatrix(const T&&);
	void display();
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value> operator+=(U&&);
	template<typename... Us>
	void addd(Us &&... u);					// without SFINAE
private:
	FriendWrapperMatrixTest;
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
WrapperMatrix<T>::pushLine(U&& newLine)
{
	if (newLine.size() == this->matrix.at(0).size())
		matrix.emplace_back(std::forward<U>(newLine));
	else
		throw std::invalid_argument("Invalid syntax");
}
template<typename T>
template<typename U>
std::enable_if_t<decay_equiv<U, std::vector<T>>::value>
WrapperMatrix<T>::pushColumn(U&& newColumn)
{
	if (newColumn.size() == this->matrix.size())
	{
		for (int i = 0; i < matrix.size(); ++i)
			matrix.at(i).emplace_back(std::forward<T>(newColumn.at(i)));
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
			std::cout << matrix.at(i).at(j) << " ";
		std::cout << std::endl;
	}
}
template<typename T>
template <typename U>
std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>>
WrapperMatrix<T>::operator+(U&& wrapperMatrix)
{
	WrapperMatrix resultWrapperMatrix(*this);
	unsigned int weightResultMatrix = resultWrapperMatrix.matrix.size();
	unsigned int weightInputMatrix = wrapperMatrix.matrix.size();
	unsigned int lengthResultMatrix = resultWrapperMatrix.matrix.at(0).size();
	unsigned int lengthInputMatrix = wrapperMatrix.matrix.at(0).size();
	if (weightResultMatrix != weightInputMatrix || lengthResultMatrix != lengthInputMatrix)
		throw std::invalid_argument("Invalid syntax");
	for (int i = 0; i < weightInputMatrix; ++i)
	{
		for (int j = 0; j < lengthInputMatrix; ++j)
		{
			resultWrapperMatrix.matrix.at(i).at(j) += std::forward<T>(wrapperMatrix.matrix.at(i).at(j));
		}
	}
	return resultWrapperMatrix;
}
template<typename T>
template <typename U>
std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>>
WrapperMatrix<T>::operator-(U&& wrapperMatrix)
{
	WrapperMatrix resultWrapperMatrix(*this);
	unsigned int weightResultMatrix = resultWrapperMatrix.matrix.size();
	unsigned int weightInputMatrix = wrapperMatrix.matrix.size();
	unsigned int lengthResultMatrix = resultWrapperMatrix.matrix.at(0).size();
	unsigned int lengthInputMatrix = wrapperMatrix.matrix.at(0).size();
	if (weightResultMatrix != weightInputMatrix || lengthResultMatrix != lengthInputMatrix)
		throw std::invalid_argument("Invalid syntax");
	for (int i = 0; i < weightInputMatrix; ++i)
	{
		for (int j = 0; j < lengthInputMatrix; ++j)
		{
			resultWrapperMatrix.matrix.at(i).at(j) -= std::forward<T>(wrapperMatrix.matrix.at(i).at(j));
		}
	}
	return resultWrapperMatrix;
}
template<typename T>
void WrapperMatrix<T>::fillMatrix(const T&& number)
{
	for (int i = 0; i < matrix.size(); ++i)
		std::fill(matrix.at(i).begin(), matrix.at(i).end(), number);
}
template<typename T>
template <typename U>
std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value>
WrapperMatrix<T>::operator+=(U&& wrapperMatrix)
{
	unsigned int weightMatrix = this->matrix.size();
	unsigned int weightInputMatrix = wrapperMatrix.matrix.size();
	unsigned int lengthMatrix = this->matrix.at(0).size();
	unsigned int lengthInputMatrix = wrapperMatrix.matrix.at(0).size();
	if (weightMatrix != weightInputMatrix || lengthMatrix != lengthInputMatrix)
		throw std::invalid_argument("Invalid syntax");
	for (int i = 0; i < weightInputMatrix; ++i)
	{
		for (int j = 0; j < lengthInputMatrix; ++j)
		{
			this->matrix.at(i).at(j) += std::forward<T>(wrapperMatrix.matrix.at(i).at(j));
		}
	}
}

template<typename T>
template<typename... Us>
void WrapperMatrix<T>::addd(Us &&... u)
{
	const int dummy[] = { 0, ((*this += std::forward<Us>(u)), 0)... };
	static_cast<void>(dummy);
}