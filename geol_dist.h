#ifndef GEOL_DIST_H
#define GEOL_DIST_H

#include <math.h>

double dist_euclidean(double x0, double y0, double x1, double y1);

double dist_manhatten(double x0, double y0, double x1, double y1);

double dist_chebyshev(double x0, double y0, double x1, double y1);

size_t NNS(
	const double* _x, const double* _y, size_t count,
	double x, double y,
	double (*cmp)(double, double, double, double)
);

size_t NNS_euclidean(
	const double* _x, const double* _y, size_t count,
	double x, double y
);

size_t NNS_manhatten(
	const double* _x, const double* _y, size_t count,
	double x, double y
);

size_t NNS_chebyshev(
	const double* _x, const double* _y, size_t count,
	double x, double y
);

#ifdef GEOL_DIST_IMPLEMENTATION

double dist_euclidean(double x0, double y0, double x1, double y1) {
	return sqrtf(powf(x1 - x0, 2) + powf(y1 - y0, 2));
}

double dist_manhatten(double x0, double y0, double x1, double y1) {
	return fabs(x1 - x0) + fabs(y1 - y0);
}

double dist_chebyshev(double x0, double y0, double x1, double y1) {
	const double dx = fabs(x1 - x0);
	const double dy = fabs(y1 - y0);
	return dx > dy ? dx : dy;
}

size_t NNS(
	const double* _x, const double* _y, size_t count,
	double x, double y,
	double (*cmp)(double, double, double, double)
) {
	size_t best = 0;
	double best_dist = SIZE_MAX;
	for (size_t i = 0; i < count; i++) {
		const double dist = cmp(x, y, _x[i], _y[i]);
		if (dist < best_dist) {
			best      = i;
			best_dist = dist;
		}
	}
	return best;
}

size_t NNS_euclidean(
	const double* _x, const double* _y, size_t count,
	double x, double y
) {
	return NNS(_x, _y, count, x, y, dist_euclidean);
}

size_t NNS_manhatten(
	const double* _x, const double* _y, size_t count,
	double x, double y
) {
	return NNS(_x, _y, count, x, y, dist_manhatten);
}

size_t NNS_chebyshev(
	const double* _x, const double* _y, size_t count,
	double x, double y
) {
	return NNS(_x, _y, count, x, y, dist_chebyshev);
}

#endif
#endif
