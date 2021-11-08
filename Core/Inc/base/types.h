/*
 * types.h
 *
 *  Created on: 2017年8月2日
 *      Author: gene
 */

#ifndef CORE_BASE_TYPES_H_
#define CORE_BASE_TYPES_H_

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#ifndef false
#define false 0
#define true 1
#endif

#ifndef null
#define null 0
#endif

#define TYPEDEF_FUNC_POINTER(name, type, ...) typedef type (*name) (__VA_ARGS__)

#define DEF_FUNC_POINTER(name, type, ...) type (*name) (__VA_ARGS__)

typedef char int_8;
typedef unsigned char uint_8;
typedef unsigned char byte;

typedef uint_8 bool_t;

typedef short int_16;
typedef unsigned short uint_16;

typedef int int_32;
typedef unsigned int uint_32;

TYPEDEF_FUNC_POINTER(callback_void_void, void, void);

#endif /* CORE_BASE_TYPES_H_ */
