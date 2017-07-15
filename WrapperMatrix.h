#pragma once
#ifndef FriendWrapperMatrixTest
#define FriendWrapperMatrixTest
#endif 
//std::enable_if_t<std::is_same<std::decay<Us>::type..., std::vector<T>>>
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
template < bool... > struct all;
template < > struct all<> : std::true_type {};
template < bool B, bool... Rest > struct all<B, Rest...>
{
	constexpr static bool value = B && all<Rest...>::value;
};

template<typename T> struct is_vector : public std::false_type {};

template<typename T, typename A>
struct is_vector<std::vector<T, A>> : public std::true_type {};

template< typename T >
class WrapperMatrix
{
public:
	WrapperMatrix(const int& weight, const int& length);
	WrapperMatrix(const std::initializer_list<std::vector<T>>&);
	template<typename U, typename = typename std::enable_if<is_vector<std::decay_t<U>>::value>::type >
	void pushLine(U&&);
	template<typename U>
	std::enable_if_t<decay_equiv<U, std::vector<T>>::value> pushColumn(U&&);
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>> operator+(U&&);
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value, WrapperMatrix<T>> operator-(U&&);
	template<typename U, typename = typename std::enable_if_t<std::is_same<T, U>::value>>
	void fillMatrix(const U&&);
	void display();
	template <typename U>
	std::enable_if_t<decay_equiv<U, WrapperMatrix<T>>::value> operator+=(U&&);
	template<typename... Us,
		typename = typename std::enable_if< all< is_WrapperMatrix< std::decay_t<Us> >::value... >::value >::type >
	void add(Us &&... u);					
	template<typename U, typename = std::enable_if_t<std::is_arithmetic<U>::value>>
	WrapperMatrix<T> operator*(U&& u);
	template <typename W>
	std::enable_if_t<decay_equiv<W, WrapperMatrix<T>>::value> multiplyWrapperMatrix(W&& w);
private:
	FriendWrapperMatrixTest;
	std::vector<std::vector<T>> matrix;
};

template<typename T> struct is_WrapperMatrix : public std::false_type {};

template<typename T>
struct is_WrapperMatrix<WrapperMatrix<T>> : public std::true_type {};

template<typename T>
WrapperMatrix<T>::WrapperMatrix(const int& weight, const int& length) : 
	matrix(std::vector<std::vector<T>>(weight, std::vector<T>(length))) {}

template<typename T>
WrapperMatrix<T>::WrapperMatrix( const std::initializer_list<std::vector<T>>& initializerList) 
	: matrix(initializerList) {}

template<typename T>
template <typename U, typename = typename std::enable_if<is_vector<std::decay_t<U>>::value>::type >
void WrapperMatrix<T>::pushLine(U&& newLine)
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
template<typename U, typename = typename std::enable_if_t<std::is_same<T,std::decay<U>::type>::value>>
void WrapperMatrix<T>::fillMatrix(const U&& number)
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
template<typename... Us, typename = typename std::enable_if< all< is_WrapperMatrix< std::decay_t<Us> >::value... >::value >::type >
void WrapperMatrix<T>::add(Us &&... u)
{
	const int dummy[] = { 0, ((*this += std::forward<Us>(u)), 0)... };
	static_cast<void>(dummy);
}
template<typename T>
template<typename U, typename = std::enable_if_t<std::is_arithmetic<U>::value>>
WrapperMatrix<T> WrapperMatrix<T>::operator*(U&& u)
{
	for (auto& itr : this->matrix)
	{
		for (auto& itritr : itr)
		{
			itritr *= u;
		}
	}
	return *this;
}
template<typename T>
template <typename W>
std::enable_if_t<decay_equiv<W, WrapperMatrix<T>>::value> WrapperMatrix<T>::multiplyWrapperMatrix(W&& w)
{
	WrapperMatrix<T> wrapperMatrix(this->matrix.size(), w.matrix.at(0).size());
	int result = 0;
	int column = 0;
	if (this->matrix.size() == w.matrix.at(0).size() && this->matrix.at(0).size() == w.matrix.size())
	{
		for (int i = 0; i < this->matrix.size(); ++i)
		{
			for (int j = 0; j < this->matrix.at(0).size(); ++j)
			{
				result += this->matrix.at(i).at(j) * w.matrix.at(j).at(column);
			}
			wrapperMatrix.matrix.at(i).at(column) = result;
			if (column != this->matrix.size() - 1)
			{
				--i;
				++column;
			}
			else
				column = 0;
			result = 0;
		}
	}
	else
		throw std::invalid_argument("Invalid syntax"); 
	this->matrix = std::move(wrapperMatrix.matrix);
}