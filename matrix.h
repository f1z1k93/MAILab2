#ifndef MAdoubleRIX_H
#define MAdoubleRIX_H 1

#include <iostream>
#include <cmath>
#define DIM 3

using namespace std;

class Point;
class Matrix {
public:
    Matrix() {}
	Matrix(int n, int m);
	Matrix(const Matrix& b);
	Matrix(double* vector, int m = DIM + 1);
    ~Matrix();

	int get_n() const { return n; }
	int get_m() const { return m; }
    const double& set(int i, int j, double val) { return matr[i][j] = val; }
    const double& get(int i, int j) const { return matr[i][j]; }
	

    friend const Matrix operator *(const Matrix& a, const Matrix& b);
 	friend const Matrix operator +(const Matrix& a, const Matrix& b);
	friend const Matrix operator -(const Matrix& a, const Matrix& b);
	friend const Matrix operator |(const Matrix& a, const Matrix& b);
    friend ostream& operator <<(ostream& cout, const Matrix& a);

	const Matrix operator ~() const;
	const Matrix operator !() const;
    double* operator[] (int i) const;
	const Matrix& operator =(const Matrix& b);
    operator Point() const;

	static const Matrix rotx(double apha);
    static const Matrix roty(double alpha);
    static const Matrix rotz(double alpha);
    static const Matrix rot(double alpha, double beta, double gama = 0);
    static const Matrix scale(double scale_x, double scale_y, double scale_z = 1);
    static const Matrix shift(double shift_x, double shift_y, double shift_z = 0);
    static const Matrix identity(int n = DIM + 1);
    int print();
protected:
	int n, m;
    double **matr;
};

class Point: public Matrix {
public:
	Point(double w = 1.0);
	Point(double x, double y, double z, double w = 1);
	Point(const Point& b);

    const Point& operator=(const Point& b);
	operator Matrix() const { return *((Matrix *) this); }
    friend const Point operator *(double a, const Point& b);
    double x() const { return matr[0][0]; }
    double y() const { return matr[1][0]; }
    double z() const { return matr[2][0]; }
    double w() const { return matr[3][0]; }
    double len() const;
};

#endif
