#ifndef MATRIX_H
#define MATRIX_H
using namespace std;

class Matrix
{
    int no_rows;
    int no_cols;
    double** value;
public:
    Matrix (int rows, int cols);

    Matrix();

    ~Matrix();

    void init(istream& in);

    void print();

    Matrix operator+ (const Matrix &m2);

    Matrix operator* (const Matrix &m2);

    Matrix& operator= (const Matrix &m2);

    bool operator== (const Matrix &m2);

    bool operator!= (const Matrix &m2);

    Matrix transpose ();

    double getValue(int i, int j);

    int getNumberOfLines();

    int getNumberOfColumns();

private:
    Matrix* appendUnitMatrix();

    void arrangeRows(int column, bool direction);

    //* row1 = row1 - k*row2
    void reduceRow(int row1, int row2, double k);

    double** allocateMatrix(int, int);


public:
    Matrix inverse ();
};


#endif // MATRIX_H
