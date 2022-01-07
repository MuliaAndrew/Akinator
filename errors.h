#ifndef ERRORS_H
#define ERRORS_H

#define ptr_ver(ptr) 					\
			if ((ptr) == nullptr)		\
				return PTR_ERROR		\

#define debug(msg) printf(#msg); \
				   printf("\n")  \

enum Error
{
	NO_ERROR 	= 0,
	PTR_ERROR 	= 1,
	LOAD_ERROR  = 2,
};

#endif // ERRORS_H
