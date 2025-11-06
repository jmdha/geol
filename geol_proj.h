#ifndef GEOL_PROJ_H
#define GEOL_PROJ_H

#define EARTH_SEMI_MAJOR_AXIS 6378137
#define EARTH_SEMI_MINOR_AXIS 6356752

void WGS84toECEF(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h,
	double  a,   double  b
);

void WGS84toECEFe(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h
);

#ifdef GEOL_PROJ_IMPLEMENTATION

#include <math.h>

void WGS84toECEF(
	double* x,   double* y,  double* z,
	double  lat, double  lon, double h,
	double  a,   double  b
) {
	const double f = 1 - b / a;
	const double E = 1 - (b * b) / (a * a);
	const double N = a / sqrt(1 - E * (sin(lat) * sin(lat)));
	*x = (N + h) * cos(lat) * cos(lon);
	*y = (N + h) * cos(lat) * sin(lon);
	*z = ((1 - f) * (1 - f) * N + h) * sin(lat);
}

void WGS84toECEFe(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h
) {
	const double a = 6378137.0;
	const double b = 6356752.3142;
	const double f = 1 - b / a;
	const double E = 1 - (b * b) / (a * a);
	const double N = a / sqrt(1 - E * (sin(lat) * sin(lat)));
	*x = (N + h) * cos(lat) * cos(lon);
	*y = (N + h) * cos(lat) * sin(lon);
	*z = ((1 - f) * (1 - f) * N + h) * sin(lat);
}

#endif
#endif
