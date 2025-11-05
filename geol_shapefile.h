#ifndef GEOL_SHAPEFILE_H
#define GEOL_SHAPEFILE_H

#include <stddef.h>

typedef enum geol_kind {
	GEOL_NULL        = 0,
	GEOL_POINT       = 1,
	GEOL_POLYLINE    = 3,
	GEOL_POLYGON     = 5,
	GEOL_MULTIPOINT  = 8,
} geol_kind_t;

typedef struct geol_record {
	geol_kind_t kind;
	size_t      part_count;
	size_t      point_count;
	size_t*     parts;
	double*     px;
	double*     py;
	double      x_min, y_min, x_max, y_max;
} geol_record_t;

size_t geol_shp_record_decode(geol_record_t* record, const char* str);
size_t geol_shp_decode(geol_record_t** records, const char* str);
size_t geol_shx_decode(size_t** offsets, size_t** lengths, const char* str);

#ifdef GEOL_SHAPEFILE_IMPLEMENTATION

#include <stdlib.h>
#include <memory.h>

size_t _geol_i32bd(const char* str) {
	return (str[3] & 0xFF)       |
	       (str[2] & 0xFF) << 8  |
	       (str[1] & 0xFF) << 16 |
	       (str[0] & 0xFF) << 24;
}

size_t _geol_i32ld(const char* str) {
	return (str[0] & 0xFF)       |
	       (str[1] & 0xFF) << 8  |
	       (str[2] & 0xFF) << 16 |
	       (str[3] & 0xFF) << 24;
}

// TODO: Actually force parsing as little endian
double _geol_f64ld(const char* str) {
	double out = 0;
	memcpy(&out, str, 8);
	return out;
}

size_t _geol_len_from_header(const char* str) {
	return 2 * _geol_i32ld(&str[24]);
}

void _geol_shp_point_decode(geol_record_t* record, const char* str) {
	record->part_count  = 1;
	record->point_count = 1;
	record->parts       = malloc(sizeof(size_t));
	record->px          = malloc(sizeof(double));
	record->py          = malloc(sizeof(double));
	record->px[0]       = _geol_f64ld(&str[0]);
	record->py[0]       = _geol_f64ld(&str[8]);
	record->x_min       = record->px[0];
	record->y_min       = record->py[0];
	record->x_max       = record->px[0];
	record->y_max       = record->py[0];
}

void _geol_shp_multipoint_decode(geol_record_t* record, const char* str) {
	record->point_count = _geol_i32ld(&str[32]);
	record->part_count  = 1;
	record->x_min       = _geol_f64ld(&str[0]);
	record->y_min       = _geol_f64ld(&str[8]);
	record->x_max       = _geol_f64ld(&str[16]);
	record->y_max       = _geol_f64ld(&str[24]);
	record->parts       = malloc(sizeof(size_t));
	record->px          = malloc(sizeof(double) * record->point_count);
	record->py          = malloc(sizeof(double) * record->point_count);
	for (size_t i = 0; i < record->point_count; i++) {
		record->px[i] = _geol_f64ld(&str[36 + 16 * i]);
		record->py[i] = _geol_f64ld(&str[44 + 16 * i]);
	}
}

void _geol_shp_polygon_decode(geol_record_t* record, const char* str) {
	record->point_count = _geol_i32ld(&str[36]);
	record->part_count  = _geol_i32ld(&str[32]);
	record->x_min       = _geol_f64ld(&str[0]);
	record->y_min       = _geol_f64ld(&str[8]);
	record->x_max       = _geol_f64ld(&str[16]);
	record->y_max       = _geol_f64ld(&str[24]);
	record->parts       = malloc(sizeof(size_t) * record->part_count);
	record->px          = malloc(sizeof(double) * record->point_count);
	record->py          = malloc(sizeof(double) * record->point_count);
	for (size_t i = 0; i < record->part_count; i++)
		record->parts[i] = _geol_i32ld(&str[40 + 4 * i]);
	for (size_t i = 0; i < record->point_count; i++) {
		record->px[i] = _geol_f64ld(&str[40 + 4 * record->part_count + 16 * i]);
		record->py[i] = _geol_f64ld(&str[48 + 4 * record->part_count + 16 * i]);
	}
}

size_t geol_shp_record_decode(geol_record_t* record, const char* str) {
	record->kind = _geol_i32ld(&str[8]);
	switch (record->kind) {
	case GEOL_POINT:       _geol_shp_point_decode(record, &str[12]);      break;
	case GEOL_MULTIPOINT:  _geol_shp_multipoint_decode(record, &str[12]); break;
	case GEOL_POLYLINE:    _geol_shp_polygon_decode(record, &str[12]);    break;
	case GEOL_POLYGON:     _geol_shp_polygon_decode(record, &str[12]);    break;
	}
	return 8 + 2 * _geol_i32bd(&str[4]);
}

size_t geol_shp_decode(geol_record_t** records, const char* str) {
	size_t count = 0;
	size_t len = _geol_len_from_header(&str[0]);
	size_t offset = 100;
	while (offset < len) {
		*records = realloc(*records, (count + 1) * sizeof(geol_record_t));
		offset += geol_shp_record_decode(&(*records)[count], &str[offset]);
		count++;
	}
	return count;

}

size_t geol_shx_decode(size_t** offsets, size_t** lengths, const char* str) {

}

#endif
#endif

