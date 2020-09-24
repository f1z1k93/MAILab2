#include <math.h>
#include "matrix.h"
#include "cstdio"
#define DIM 3


Matrix::Matrix(int n, int m): n(n), m(m)
{
    matr = new double*[n];
    for (int i = 0; i < n; i++) {
        matr[i] = new double[m];
        for (int j = 0; j < m; j++) {
            matr[i][j] = 0;
        }
    }
}


Matrix::Matrix(double* vector, int cln_num): n(cln_num), m(1)
{
	matr = new double*[n];
	for (int i = 0; i < n; i++) {
		matr[i] = new double[1];
		matr[i][0] = vector[i];
	}
}


Matrix::Matrix(const Matrix& b): n(b.n), m(b.m)
{
    matr = new double*[n];
    for (int i = 0; i < n; i++) {
        matr[i] = new double[m];
        for (int j = 0; j < m; j++) {
            matr[i][j] = b.matr[i][j];
        }
    }
}


Matrix::~Matrix()
{
    if (matr == 0)
        return;
    for (int i = 0; i < n; i++) {
        if (matr[i] != 0) {
            delete[] matr[i];
            matr[i] = 0;
        }
    }
    delete[] matr;
    matr = 0;
}

ostream& operator <<(ostream& cout, const Matrix& a)
{
	for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            printf("%7.2lf ", a.matr[i][j]);
        }
		printf("\n");
	}
	return cout;
}


const Matrix operator+ (const Matrix& a, const Matrix& b)
{
    Matrix c(a);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
                c.matr[i][j] += b.matr[i][j];
        }
    }
    return c;
}


const Matrix operator- (const Matrix& a, const Matrix& b)
{
    Matrix c(a);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
                c.matr[i][j] -= b.matr[i][j];
        }
    }
    return c;
}



const Matrix operator* (const Matrix& a, const Matrix& b)
{
    Matrix c(a.n, b.m);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < b.m; j++) {
            for (int r = 0; r < a.m; r++) {
                c.matr[i][j] += a.matr[i][r] * b.matr[r][j];
            }
        }
    }
    return c;
}

const Point operator* (double a, const Point& b)
{
    Point c;
    for (int i = 0; i < DIM; i++) {
            c.matr[i][0] = a * b.matr[i][0];
    }
    return c;
}

const Matrix operator |(const Matrix& a, const Matrix& b)
{
	Matrix c(a.n, a.m + b.m);
	for (int i = 0; i < c.n; i++) {
		for (int j = 0; j < a.m; j++)
			c.matr[i][j] = a.matr[i][j];
		for (int j = 0; j < b.m; j++)
			c.matr[i][a.m + j] = b.matr[i][j];
	}
	return c;
}

const Matrix& Matrix::operator =(const Matrix& b)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matr[i][j] = b.matr[i][j];
    return (*this);
}

const Matrix Matrix::operator ~() const
{
	Matrix c(m, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			c.matr[j][i] = matr[i][j];
	}
	return c;
}

void to_up(double **a, int k, int m)
{
	for (int i = k; i < m; i++) {
		if (a[i][k] != 0) {
			for (int j = 0; j < m; j++) {
				double t = a[i][j];
				a[i][j] = a[k][j];
				a[k][j] = t;
			}
			return;
		}
	}
	return;
}

void to_mul_line(double a, double* v, int m)
{
	
	for (int i = 0; i < m; i++)
		v[i] *= a;
	return;	
}

void to_add_mul_line(double *v1, double a, double* v2, int m)
{
	for (int i = 0; i < m; i++)
		v1[i] += a * v2[i];
	return;
}

const Matrix Matrix::operator !() const
{
	Matrix c = (*this) | identity(m);
	for (int k = 0; k < n; k++) {
		to_up(c.matr, k, c.m);
		to_mul_line(1 / c[k][k], c.matr[k], c.m);
		for (int i = k + 1; i < n; i++)
			to_add_mul_line(c.matr[i], -c.matr[i][k], c.matr[k], c.m);
	}

	for (int j = m - 1; j >= 0; j--) {
		for (int i = j - 1; i >= 0; i--)
			to_add_mul_line(c.matr[i], -c.matr[i][j], c.matr[j], c.m);
	}

	Matrix res(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			res.matr[i][j] = c.matr[i][m + j];
	}
	return res;
}

double* Matrix::operator[] (int i) const
{
    return matr[i];
}

Matrix::operator Point() const
{
    const Point& c = Point(matr[0][0], matr[1][0], matr[2][0], matr[3][0]);
    return c;
}

const Matrix Matrix::rotx(double alpha)
{
    Matrix matr(DIM + 1, DIM + 1);
    matr.set(0, 0, 1);
    matr.set(1, 1, cos(alpha));
    matr.set(2, 2, cos(alpha));
    matr.set(3, 3, 1);
    matr.set(1, 2, -sin(alpha));
    matr.set(2, 1, sin(alpha));
    return matr;
}

const Matrix Matrix::roty(double alpha)
{
    Matrix matr(DIM + 1, DIM + 1);
    matr.set(0, 0, cos(alpha));
    matr.set(1, 1, 1);
    matr.set(2, 2, cos(alpha));
    matr.set(3, 3, 1);
    matr.set(2, 0, -sin(alpha));
    matr.set(0, 2, sin(alpha));
    return matr;
}

const Matrix Matrix::rotz(double alpha)
{
    Matrix matr(DIM + 1, DIM + 1);
    matr.set(0, 0, cos(alpha));
    matr.set(1, 1, cos(alpha));
    matr.set(2, 2, 1);
    matr.set(3, 3, 1);
    matr.set(0, 1, -sin(alpha));
    matr.set(1, 0, sin(alpha));
    return matr;

}

const Matrix Matrix::rot(double alpha, double beta, double gama)
{
	Matrix m = rotx(alpha) * roty(beta) * rotz(gama);
	return m;
}


const Matrix Matrix::scale(double scale_x, double scale_y, double scale_z)
{
    Matrix matr(DIM + 1, DIM + 1);
    matr.set(0, 0, scale_x);
    matr.set(1, 1, scale_y);
    matr.set(2, 2, scale_z);
    matr.set(3, 3, 1);
    return matr;
}

int Matrix::print()
{
    cout << (*this);
    return 0;
}

const Matrix Matrix::shift(double shift_x , double shift_y, double shift_z)
{
    Matrix matr(DIM + 1, DIM + 1);
    matr.set(0, 3, shift_x);
    matr.set(1, 3, shift_y);
    matr.set(2, 3, shift_z);
    matr.set(0, 0, 1);
    matr.set(1, 1, 1);
    matr.set(2, 2, 1);
    matr.set(3, 3, 1);
    return matr;
}

const Matrix Matrix::identity(int n)
{
	Matrix matr(n, n);
	for (int i = 0; i < n; i++)
		matr.set(i, i, 1);
	return matr;
}


Point::Point(double w)
{
	this->n = DIM + 1;
	this->m = 1;
	matr = new double* [n];
	for (int i = 0; i < n; i++)
		matr[i] = new double[m];
	matr[n - 1][0] = w;
}

Point::Point(const Point& b)
{
    this->n = b.n;
    this->m = b.m;
    matr = new double*[n];
    for (int i = 0; i < n; i++) {
        matr[i] = new double[m];
        matr[i][0] = b.matr[i][0];
    }
}

Point::Point(double x, double y, double z, double w)
{
	this->n = DIM + 1;
	this->m = 1;
	matr = new double*[4];
	matr[0] = new double[1]; matr[0][0] = x; 
	matr[1] = new double[1]; matr[1][0] = y; 
	matr[2] = new double[1]; matr[2][0] = z; 
    matr[3] = new double[1]; matr[3][0] = w;
    return;
}

const Point& Point::operator =(const Point& b)
{
    this->n = b.n;
    this->m = b.m;
    matr[0][0] = b.matr[0][0];
    matr[1][0] = b.matr[1][0];
    matr[2][0] = b.matr[2][0];
    matr[3][0] = b.matr[3][0];
    return (*this);
}

double Point::len() const
{
    double x_2 = matr[0][0] * matr[0][0];
    double y_2 = matr[1][0] * matr[1][0];
    double z_2 = matr[2][0] * matr[2][0];
    return sqrt(x_2 + y_2 + z_2);
}

