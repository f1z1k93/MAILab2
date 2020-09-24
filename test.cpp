#include "matrix.h"
#include <iostream>
using namespace std;

int main()
{
	Matrix a = Matrix::rotx(2) * Matrix::scale(1, 2, 3) * Matrix::shift(2, 3, 1);
	Matrix b = a * (!a);
	cout << b;
	return 0;
}
