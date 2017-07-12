#pragma once
#include <gtest\gtest.h>
#include <memory>
#define FriendWrapperMatrixTest \
	FRIEND_TEST(wrapperMatrixTest, shouldAddLineToMatrixRValue);\
	FRIEND_TEST(wrapperMatrixTest, shouldAddLineToMatrixLValue);\
	FRIEND_TEST(wrapperMatrixTest, shouldAddColumnToMatrixRValue);\
	FRIEND_TEST(wrapperMatrixTest, shouldAddColumnToMatrixLValue);\
	FRIEND_TEST(wrapperMatrixTest, shouldFillMatrixOneNumber);\
	FRIEND_TEST(wrapperMatrixTest, shouldAddSeveralLineAndColumnToMatrix);\
	FRIEND_TEST(wrapperMatrixTest, shouldAddTwoVectorOfVectorsToThirdVectorOfVectors);\
	FRIEND_TEST(wrapperMatrixTest, shouldSubtractTwoVectorOfVectorsToThirdVectorOfVectors);\
	FRIEND_TEST(wrapperMatrixTest, shouldAddALotOfWrapperMatrix);
#include "WrapperMatrix.h"
class wrapperMatrixTest : public ::testing::Test
{
protected:
	void SetUp();
	std::unique_ptr<WrapperMatrix<int>> wrapperMatrixInt;
};

void wrapperMatrixTest::SetUp()
{
	wrapperMatrixInt = std::make_unique<WrapperMatrix<int>>(WrapperMatrix<int>(2, 2));
}

TEST_F(wrapperMatrixTest, shouldAddLineToMatrixRValue)
{
	std::vector<std::vector<int>> matrix{ {0,0},{0,0},{1,2} };
	std::vector<int> vec1{ 1,2};
	wrapperMatrixInt->pushLine(std::move(vec1));
	EXPECT_EQ(wrapperMatrixInt->matrix, matrix);
}
TEST_F(wrapperMatrixTest, shouldAddLineToMatrixLValue)
{
	std::vector<std::vector<int>> matrix{ { 0,0 },{ 0,0 },{ 1,2 } };
	std::vector<int> vec1{ 1,2 };
	wrapperMatrixInt->pushLine(vec1);
	EXPECT_EQ(wrapperMatrixInt->matrix, matrix);
}
TEST_F(wrapperMatrixTest, shouldAddColumnToMatrixRValue)
{
	std::vector<std::vector<int>> matrix{ { 0,0,1 },{ 0,0,2 } };
	std::vector<int> vec1{ 1,2 };
	wrapperMatrixInt->pushColumn(std::move(vec1));
	EXPECT_EQ(wrapperMatrixInt->matrix, matrix);
}
TEST_F(wrapperMatrixTest, shouldAddColumnToMatrixLValue)
{
	std::vector<std::vector<int>> matrix{ { 0,0,1 },{ 0,0,2 } };
	std::vector<int> vec1{ 1,2 };
	wrapperMatrixInt->pushColumn(vec1);
	EXPECT_EQ(wrapperMatrixInt->matrix, matrix);
}
TEST_F(wrapperMatrixTest, shouldFillMatrixOneNumber)
{
	std::vector<std::vector<int>> matrix{ { 0,0 },{ 0,0 } };
	wrapperMatrixInt->fillMatrix(1);
}
TEST_F(wrapperMatrixTest, shouldAddSeveralLineAndColumnToMatrix)
{
	std::vector<std::vector<int>> matrix{ { 0,0,2 },{ 0,0,3 }, {1,2,4},{1,2,5},{2,3,6} };
	std::vector<int> v1{ 1,2 };
	std::vector<int> v2{ 1,2 };
	std::vector<int> v3{ 2,3 };
	std::vector<int> v4{ 2,3,4,5,6 };
	wrapperMatrixInt->pushLine(v1);
	wrapperMatrixInt->pushLine(v2);
	wrapperMatrixInt->pushLine(std::move(v3));
	wrapperMatrixInt->pushColumn(std::move(v4));
	EXPECT_EQ(wrapperMatrixInt->matrix, matrix);
}
TEST_F(wrapperMatrixTest, shouldThrowExceptionDuringPushLine)
{
	std::vector<int> v1{ 2,3,4,5,6 };
	ASSERT_THROW(wrapperMatrixInt->pushLine(v1),std::invalid_argument);
}
TEST_F(wrapperMatrixTest, shouldThrowExceptionDuringPushColumn)
{
	std::vector<int> v1{ 2,3,4,5,6 };
	ASSERT_THROW(wrapperMatrixInt->pushColumn(v1), std::invalid_argument);
}
TEST_F(wrapperMatrixTest, shouldAddTwoVectorOfVectorsToThirdVectorOfVectors)
{
	WrapperMatrix<int> resultWrapperMatrix(5, 5);
	resultWrapperMatrix.fillMatrix(1);
	WrapperMatrix<int> wrapperMatrix1(5, 5);
	WrapperMatrix<int> wrapperMatrix2(5, 5);
	WrapperMatrix<int> wrapperMatrix3(5, 5);
	wrapperMatrix2.fillMatrix(1);
	wrapperMatrix3 = wrapperMatrix1 + std::move(wrapperMatrix2);
	EXPECT_EQ(resultWrapperMatrix.matrix, wrapperMatrix3.matrix);
}
TEST_F(wrapperMatrixTest, shouldDisplayMatrix)
{
	wrapperMatrixInt->display();
}
TEST_F(wrapperMatrixTest, shouldSubtractTwoVectorOfVectorsToThirdVectorOfVectors)
{
	WrapperMatrix<int> resultWrapperMatrix(5, 5);
	resultWrapperMatrix.fillMatrix(-1);
	WrapperMatrix<int> wrapperMatrix1(5, 5);
	WrapperMatrix<int> wrapperMatrix2(5, 5);
	WrapperMatrix<int> wrapperMatrix3(5, 5);
	wrapperMatrix2.fillMatrix(1);
	wrapperMatrix3 = wrapperMatrix1 - std::move(wrapperMatrix2);
	EXPECT_EQ(resultWrapperMatrix.matrix, wrapperMatrix3.matrix);
}
TEST_F(wrapperMatrixTest, shouldAddALotOfWrapperMatrix)
{
	WrapperMatrix<int> wrapperMatrix1(2, 2);
	WrapperMatrix<int> wrapperMatrix2(2, 2);
	WrapperMatrix<int> wrapperMatrix3(2, 2);
	wrapperMatrix1.fillMatrix(1);
	wrapperMatrix2.fillMatrix(1);
	wrapperMatrix3.fillMatrix(2);
	wrapperMatrixInt->addd(wrapperMatrix2, wrapperMatrix1);
	EXPECT_EQ(wrapperMatrixInt->matrix, wrapperMatrix3.matrix);
}