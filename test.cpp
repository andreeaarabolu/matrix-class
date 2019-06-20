#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include "Matrix.hpp"
using namespace std;

ifstream in1("matrix-1.in");
ifstream in2("matrix-2.in");
ifstream in3("matrix-3.in");
ifstream in4("unit3-matrix.in");
ifstream in5("matrix-4.in");

Matrix m1; //m=n=3 && det=0
Matrix m2; //m=n=3 && det!=0
Matrix m3; //m=n=4
Matrix m4; //m!=n
Matrix unit; //unit matrix

char* testsDescription[] = {
    "Does not allow the inverse of a matrix with no_rows != no_cols",
    "Successfully inversed a valid matrix",
    "Does not allow the inverse of a singular matrix",
    "Does not allow summing incompatible matrix",
    "Successfully summed two compatible matrix",
    "Does not allow multipling incompatible matrix",
    "Successfully multipied of two compatible matrix",
    "Testin '=' operator",
    "Testing the transpose"
};

void readData()
{
    m1.init(in1);
    m2.init(in2);
    m3.init(in3);
    m4.init(in5);
    unit.init(in4);
    //unit.print();
    //m1.print();
    //m2.print();
    //m3.print();
}


bool test1()
{
    try {
        m4.inverse();
        return false;
    } catch(exception& e) {
        return strcmp(e.what(),"ERROR: Matricea nu este patratica => nu se poate obtine ivnersa \n") == 0;
    }
}

bool test2()
{
    try {
        Matrix inverse = m2.inverse();
        return inverse*m2 == unit;
    } catch(exception& e) {
        return false;
    }
}

bool test3()
{
    try {
        Matrix inverse = m1.inverse();
        return false;
    } catch(exception& e) {
        return strcmp(e.what(),"ERROR: Matricea este singulara => determinant 0 \n") == 0;
    }
}

bool test4()
{
    try {
        m1 + m3;
        return false;
    } catch(exception& e) {
        return strcmp(e.what(), "ERROR: Matricele nu au aceeasi dimensiune \n") == 0;
    }
}

bool test5()
{
    try {
        Matrix sum = m1 + m2;
        return sum == m1 + m2;
    } catch(exception& e) {
        return false;
    }
}

bool test6()
{
    try {
        m1 * m3;
        return false;
    } catch(exception& e) {
        return strcmp(e.what(), "ERROR: Matricele nu se pot inmulti din cauza dimensiunilor \n") == 0;
    }
}

bool test7()
{
    try {
        Matrix prod = m1 * m2;
        return prod == m1 * m2;
    } catch(exception& e) {
        return false;
    }
}

bool test8()
{
    Matrix m_copy = m1;
    return m_copy == m1;
}

bool test9()
{
    Matrix transpose = m1.transpose();
    int no_rows = m1.getNumberOfLines();
    int no_cols = m1.getNumberOfColumns();
    for(int i = 0; i < no_rows; i++)
    {
        for(int j = 0; j < no_cols; j++)
            if(m1.getValue(i,j) != transpose.getValue(j,i))
            {
                return false;
            }
    }
    return true;
}

void runTests()
{
    readData();
    const int numberOfTests = sizeof(testsDescription)/sizeof(char*);
    bool (* tests[])() = {test1, test2, test3, test4, test5, test6, test7, test8, test9};
    for(int i = 0; i < numberOfTests; i++)
    {
        printf("%d) %s --> ",i+1,testsDescription[i]);
        if(tests[i]() == false)
        {
            printf("FAIL\n");
        }
        else
        {
            printf("SUCCESS\n");
        }
    }
}
