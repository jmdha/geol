#include <stdio.h>
#include <assert.h>

#define GEOL_PROJ_IMPLEMENTATION
#include "../geol_proj.h"

typedef struct vec3 {
	double x, y, z;
} vec3_t;

bool cmp(vec3_t a, vec3_t b) {
	return round(a.x) == round(b.x) && 
	       round(a.y) == round(b.y) && 
	       round(a.z) == round(b.z);
}

void test_WGS84toECEF() {
	printf("----WGS84toECEF----\n");
	const size_t count = 17;
	const vec3_t tests[] = {
		{.x =   0, .y =    0, .z = 0},
		{.x = -90, .y =    0, .z = 0},
		{.x = -45, .y =    0, .z = 0},
		{.x =  45, .y =    0, .z = 0},
		{.x =  90, .y =    0, .z = 0},
		{.x =   0, .y = -180, .z = 0},
		{.x =   0, .y =  -90, .z = 0},
		{.x =   0, .y =   90, .z = 0},
		{.x =   0, .y =  180, .z = 0},
		{.x = -45, .y =  -45, .z = 0},
		{.x = -45, .y =   45, .z = 0},
		{.x =  45, .y =  -45, .z = 0},
		{.x =  45, .y =   45, .z = 0},
		{.x = -45, .y =  -45, .z = 100},
		{.x = -45, .y =   45, .z = 100},
		{.x =  45, .y =  -45, .z = 100},
		{.x =  45, .y =   45, .z = 100},
	};
	const vec3_t expected[] = {
		{.x = 6378137, .y = 0, .z = 0},
		{.x = 0, .y = 0, .z = -6356752},
		{.x = 4517591, .y = 0, .z = -4487348},
		{.x = 4517591, .y = 0, .z = 4487348},
		{.x = 0, .y = 0, .z = 6356752},
		{.x = -6378137, .y = 0, .z = 0},
		{.x =   0, .y = -6378137, .z = 0},
		{.x =   0, .y = 6378137, .z = 0},
		{.x = -6378137, .y = 0, .z = 0},
		{.x = 3194419, .y = -3194419, .z = -4487348},
		{.x = 3194419, .y = 3194419, .z = -4487348},
		{.x = 3194419, .y = -3194419, .z = 4487348},
		{.x = 3194419, .y = 3194419, .z = 4487348},
		{.x = 3194469, .y = -3194469, .z = -4487419},
		{.x = 3194469, .y = 3194469, .z = -4487419},
		{.x = 3194469, .y = -3194469, .z = 4487419},
		{.x = 3194469, .y = 3194469, .z = 4487419},
	};
	for (size_t i = 0; i < count; i++) {
		printf("%zu...", i);
		fflush(stdout);
		vec3_t a;
		WGS84toECEF_deg(&a.x, &a.y, &a.z, tests[i].x, tests[i].y, tests[i].z);
		assert(cmp(a, expected[i]));
		printf("ok\n");
	}
}

int main(int argc, char** argv) {
	test_WGS84toECEF();
}
