#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

Vector::Vector()
{
	data = new double[3];
	data[0] = 0.0;
	data[1] = 0.0;
	data[2] = 1.0;
}

Vector::Vector(const Vector& oldVector)
{
	data = new double[3];
	data[0] = oldVector[0];
	data[1] = oldVector[1];
	data[2] = oldVector[2];
}

Vector::Vector(const double x, const double y)
{
	data = new double[3];
	data[0] = x;
	data[1] = y;
	data[2] = 1;
}


Vector::~Vector()
{
	delete[] data;
}

double& Vector::operator[](int index) const
{
	return data[index];
}

Matrix::Matrix()
{
	data = new double*[3];
	for (int i = 0; i < 3; i++) {
		data[i] = new double[3];
		for (int j = 0; j < 3; j++) {
			data[i][j] = i == j ? 1 : 0;
		}
	}
}

Matrix::Matrix(const Matrix& oldMatrix)
{
	data = new double*[3];
	for (int i = 0; i < 3; i++) {
		data[i] = new double[3];
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			data[i][j] = oldMatrix[i][j];
		}
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < 3; i++) delete data[i];
	delete data;
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	Matrix* prod = new Matrix();
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			(*prod)[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				(*prod)[i][j] += data[i][k] * (*otherMatrix)[k][j];
			}
		}
	}
	return prod;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
	Vector* vector = new Vector;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			(*vector)[i] += data[i][j] * (*theVector)[j];
		}
	}
	return vector;
}

double* Matrix::operator[](int index) const
{
	return data[index];
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* translate = new Matrix();

	(*translate)[0][0] = 1;
	(*translate)[0][1] = 0;
	(*translate)[0][2] = deltaX;
	(*translate)[1][0] = 0;
	(*translate)[1][1] = 1;
	(*translate)[1][2] = deltaY;
	(*translate)[2][0] = 0;
	(*translate)[2][1] = 0;
	(*translate)[2][2] = 1;

	return translate;
}

Matrix* Matrix::rotation(double theta)
{
	Matrix* rotation = new Matrix();

	(*rotation)[0][0] = cos(theta);
	(*rotation)[0][1] = -sin(theta);
	(*rotation)[0][2] = 0;
	(*rotation)[1][0] = sin(theta);
	(*rotation)[1][1] = cos(theta);
	(*rotation)[1][2] = 0;
	(*rotation)[2][0] = 0;
	(*rotation)[2][1] = 0;
	(*rotation)[2][2] = 1;

	return rotation;

}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	{
		Matrix* shearing = new Matrix();
		(*shearing)[0][0] = 1;
		(*shearing)[0][1] = shearXY;
		(*shearing)[0][2] = 0;
		(*shearing)[1][0] = shearYX;
		(*shearing)[1][1] = 1;
		(*shearing)[1][2] = 0;
		(*shearing)[2][0] = 0;
		(*shearing)[2][1] = 0;
		(*shearing)[2][2] = 1;

		return shearing;
	}
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	Matrix* scale = new Matrix();
	(*scale)[0][0] = scaleX;
	(*scale)[0][1] = 0;
	(*scale)[0][2] = 0;
	(*scale)[1][0] = 0;
	(*scale)[1][1] = scaleY;
	(*scale)[1][2] = 0;
	(*scale)[2][0] = 0;
	(*scale)[2][1] = 0;
	(*scale)[2][2] = 1;

	return scale;
}

Matrix* Matrix::getInverse() const
{
	Matrix* answer = new Matrix();
	double det = getDeterminant();

	answer->data[0][0] = -data[1][2] * data[2][1] + data[1][1] * data[2][2];
	answer->data[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	answer->data[0][2] = -data[0][2] * data[1][1] + data[0][1] * data[1][2];
	answer->data[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	answer->data[1][1] = -data[0][2] * data[2][0] + data[0][0] * data[2][2];
	answer->data[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
	answer->data[2][0] = -data[1][1] * data[2][0] + data[1][0] * data[2][1];
	answer->data[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
	answer->data[2][2] = -data[0][1] * data[1][0] + data[0][0] * data[1][1];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			answer->data[i][j] /= det;

	return answer;
}

double Matrix::getDeterminant() const
{
	return  -data[0][2] * data[1][1] * data[2][0] + data[0][1] * data[1][2] * data[2][0] +
		data[0][2] * data[1][0] * data[2][1] - data[0][0] * data[1][2] * data[2][1] -
		data[0][1] * data[1][0] * data[2][2] + data[0][0] * data[1][1] * data[2][2];
}
