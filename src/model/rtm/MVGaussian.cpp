#include "stdafx.h"
#include "MVGaussian.h"
#include "utils_rtm.h"
#include "defs.h"
#include "cokus.h"

MVGaussian::MVGaussian(void)
{
	m_iSet = 0;
}

MVGaussian::~MVGaussian(void)
{
}

void MVGaussian::nextMVGaussian(double *mean, double **precision, double *res, const int &n) 
{
	double **precisionLowerTriangular = new double*[n];
	for ( int i=0; i<n; i++ ) {
		precisionLowerTriangular[i] = new double[n];
	}
	choleskydec(precision, precisionLowerTriangular, n, false);

	return nextMVGaussianWithCholesky(mean, precisionLowerTriangular, res, n);
}

void MVGaussian::nextMVGaussianWithCholesky(double *mean, double **precisionLowerTriangular, double *res, const int &n) 
{
	// Initialize vector z to standard normals
	//  [NB: using the same array for z and x]
	for (int i = 0; i < n; i++) {
		res[i] = nextGaussian();
	}

	// Now solve trans(L) x = z using back substitution
	double innerProduct;

	for (int i = n-1; i >= 0; i--) {
		innerProduct = 0;
		for (int j = i+1; j < n; j++) {
			// the cholesky decomp got us the precisionLowerTriangular triangular
			//  matrix, but we really want the transpose.
			innerProduct += res[j] * precisionLowerTriangular[j][i];
		}
		res[i] = (res[i] - innerProduct) / precisionLowerTriangular[i][i];
	}

	for (int i = 0; i < n; i++) {
		res[i] += mean[i];
	}
}

double MVGaussian::nextGaussian()
{
	if ( m_iSet == 0 ) {
		double dRsq = 0;
		double v1, v2;
		do {
			v1 = 2.0 * myrand() - 1.0;
			v2 = 2.0 * myrand() - 1.0;
			dRsq = v1 * v1 + v2 * v2;
		} while (dRsq > 1.0 || dRsq < 1e-300);

		double dFac = sqrt(-2.0 * log(dRsq) / dRsq);
		m_dGset = v1 * dFac;
		m_iSet = 1;
		return v2 * dFac;
	} else {
		m_iSet = 0;
		return m_dGset;
	}
}