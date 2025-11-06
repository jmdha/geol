#ifndef GEOL_PROJ_H
#define GEOL_PROJ_H

void WGS84toECEF_deg(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h
);

void WGS84toECEF_rad(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h
);

#ifdef GEOL_PROJ_IMPLEMENTATION

#include <math.h>

#define _geol_pi 3.1415926535898
#define _geol_a  6378137.0
#define _geol_b  6356752.3142
#define _geol_f  0.00335281066475
#define _geol_E  0.00669437999014

#define _geol_degtorad(deg) ((deg) * _geol_pi / 180.0)
#define _geol_radtodeg(rad) ((rad) * 180.0 / _geol_pi)

void WGS84toECEF_deg(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h
) {
	WGS84toECEF_rad(
		x, y, z,
		_geol_degtorad(lat),
		_geol_degtorad(lon),
		h
	);
}

void WGS84toECEF_rad(
	double* x,   double* y,   double* z,
	double  lat, double  lon, double  h
) {
	const double N = _geol_a / sqrt(1 - _geol_E * sin(lat) * sin(lat));

	*x = (h + N) * cos(lat) * cos(lon);
	*y = (h + N) * cos(lat) * sin(lon);
	*z = (h + N * (1 - _geol_E)) * sin(lat);
}

#endif
#endif
