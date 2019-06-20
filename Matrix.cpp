#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cmath>
#include "Matrix.hpp"
using namespace std;

const double EPS = 0.000001;

double** Matrix::allocateMatrix(int rows, int cols)
{
    double** value = new double*[rows];
    for (int i = 0; i < no_rows; i++)
        value[i] = new double [cols];
    return value;
}

Matrix::Matrix (int rows, int cols)
{
    no_rows = rows;
    no_cols = cols;
    value = allocateMatrix(rows, cols);
}

Matrix::Matrix()
{
    no_rows = 0;
    no_cols = 0;
    value = NULL;
}

Matrix::~Matrix()
{
    for (int i = 0; i < no_rows; i++)
        delete value[i];
    delete value;
}

void Matrix::init(istream& in)
{
    in>>no_rows;
    in>>no_cols;
    value = allocateMatrix(no_rows, no_cols);
    for (int i = 0; i < no_rows; i++)
        for (int j = 0; j < no_cols; j++)
            in >> value[i][j];
}

void Matrix::print()
{
    cout <<"Matrix: ";
    for (int i = 0; i < no_rows; i++)
    {
        cout << endl;
        for (int j = 0; j < no_cols; j++)
            cout<< value[i][j] << " ";
    }
    cout << endl;
}

Matrix Matrix::operator+ (const Matrix &m2)
{
    Matrix *result = new Matrix(no_rows,no_cols);
    if (no_cols != m2.no_cols || no_rows != m2.no_rows)
        throw invalid_argument( "ERROR: Matricele nu au aceeasi dimensiune \n");
    else
    {
        for (int i = 0; i < no_rows; i++)
            for (int j = 0; j < no_cols; j++)
                result->value[i][j] = value[i][j] + m2.value[i][j];
    }
    return *result;
}

Matrix Matrix::operator* (const Matrix &m2)
{
    Matrix *result = new Matrix(no_rows, m2.no_cols);
    if (no_cols != m2.no_rows)
        throw invalid_argument("ERROR: Matricele nu se pot inmulti din cauza dimensiunilor \n");
    else
    {
        for (int i = 0; i < no_rows; i++)
            for (int j = 0; j < m2.no_cols; j++)
            {
                result->value[i][j] = 0;
                for (int k = 0; k < no_cols; k++)
                    result->value[i][j] += value[i][k] * m2.value[k][j];
                if(result->value[i][j] < EPS)
                {
                    result->value[i][j] = 0;
                }
            }
        return *result;
    }
}

Matrix& Matrix::operator= (const Matrix &m2)
{
    cout<<"equals operator called";
    this->no_cols = m2.no_cols;
    this->no_rows = m2.no_rows;
    this->value = allocateMatrix(this->no_cols, this->no_rows);
    for (int i = 0; i < m2.no_rows; i++)
        for (int j = 0; j < m2.no_cols; j++)
            value[i][j] = m2.value[i][j];
    return *this;
}

Matrix Matrix::transpose ()
{
    Matrix *result = new Matrix(no_cols, no_rows);
    for (int i = 0; i < no_rows; i++)
        for (int j = 0; j < no_cols; j++)
            result->value[j][i] = value[i][j];
    return *result;
}

Matrix Matrix::inverse ()
{
    if (no_cols != no_rows)
        throw invalid_argument("ERROR: Matricea nu este patratica => nu se poate obtine ivnersa \n");
    Matrix* gauss = this->appendUnitMatrix();
    ///gauss->print();

    // 0 below the diagonal
    for(int j = 0; j < no_cols - 1; j++)
    {
        gauss->arrangeRows(j,false);
        //cout<<"After arange rows: "<<endl;
        //gauss->print();
        //gauss->print();
        for(int i = j + 1; i < no_rows; i++)
        {
            if(abs(gauss->value[j][j]) > EPS)
            {
                double temp = gauss->value[i][j] / gauss->value[j][j];
                //cout<<"After reduce: "<<endl;
                gauss->reduceRow(i, j, temp);
                //gauss->print();
            }
        }
    }
    //cout<<"After reducing below diagonal"<<endl;
    //gauss->print();
    //0 above the diagonal
    for(int j = no_cols - 1; j >= 0; j--)
    {
        for(int i = j - 1; i >= 0; i--)
        {
            if(abs(gauss->value[j][j]) > EPS)
            {
                double temp = gauss->value[i][j] / gauss->value[j][j];
                gauss->reduceRow(i, j, temp);
                //cout<<"After reduce second for"<<endl;
                //gauss->print();
            }
        }
    }
    // 1 on the diagonal
    for (int i = 0; i < no_rows; i++)
    {
        double temp = gauss->value[i][i];
        for (int j = 0; j < 2 * no_cols; j++)
        {
            if(abs(temp) < EPS)
            {
                throw invalid_argument("ERROR: Matricea este singulara => determinant 0 \n");
            }
            gauss->value[i][j] = gauss->value[i][j] / temp;
        }
    }
    //cout<<"After normalizing the diagonal"<<endl;
    //gauss->print();

    // Extract inverse
    Matrix *result = new Matrix(no_rows, no_cols);
    for (int i = 0; i < no_rows; i++)
        for (int j = 0; j < no_cols; j++)
            result->value[i][j] = gauss->value[i][j+no_cols];

    return *result;
}


Matrix* Matrix::appendUnitMatrix()
{
    Matrix *gauss = new Matrix(no_rows, 2*no_cols);
    for (int i = 0; i < no_rows; i++)
        for (int j = 0; j < no_cols; j++)
            gauss->value[i][j] = value[i][j];
    for (int i = 0; i < no_rows; i++)
        for (int j = 0; j < no_cols; j++)
        {
            if (j == i)
                gauss->value[i][no_cols+j] = 1;
            else
                gauss->value[i][no_cols+j] = 0;
        }
    return gauss;
}

// false -> direction is top down
// true -> direction is bottowm up
void Matrix::arrangeRows(int column, bool direction)
{
    int step = (direction == false) ? -1 : 1;
    for (int i = no_rows - 1; i > 0; i--)
    {
        if (abs(value[i - 1][column]) < EPS)
        {
            double *temp = value[i];
            value[i] = value[i - 1];
            value[i - 1] = temp;
        }
    }
}

//* row1 = row1 - k*row2
void Matrix::reduceRow(int row1, int row2, double k)
{
    for (int i = 0; i < no_cols; i++)
    {
        value[row1][i] -= value[row2][i] * k;
        if(abs(value[row1][i]) < EPS)
        {
            value[row1][i] = 0;
        }
    }
}

bool Matrix::operator== (const Matrix &m2)
{
    if(no_cols != m2.no_cols || no_rows != m2.no_rows)
        return false;
    for(int i = 0; i < no_rows; i++)
    {
        for(int j = 0; j < no_cols; j++)
        {
            if(abs(value[i][j] - m2.value[i][j]) > EPS)
            {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!= (const Matrix &m2)
{
    return !(*this == m2);
}

double Matrix::getValue(int i, int j)
{
    return value[i][j];
}

int Matrix::getNumberOfColumns()
{
    return no_cols;
}

int Matrix::getNumberOfLines()
{
    return no_rows;
}
