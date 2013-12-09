/*
 ***************************************************************************
 * \brief   Header for the Orchid-IO Board
 * \file    types.h
 * \version 1.0
 * \date    28.09.2008
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.09.2008   Initial release
 *
 ***************************************************************************
 */

#ifndef TYPES_H_
#define TYPES_H_

/*
 ****************************************************************************
 *
 ****************************************************************************
 */

typedef signed char			INT8;
typedef unsigned char		UINT8;
typedef signed short		INT16;
typedef unsigned short		UINT16;
typedef signed long			INT32;
typedef unsigned long		UINT32;
typedef signed long long	INT64;
typedef unsigned long long	UINT64;
typedef float				FLOAT32;
typedef double				FLOAT64;
typedef unsigned char		BOOL;
typedef char				CHAR;

#define FALSE			0
#define	TRUE			1

#ifndef NULL
 #define NULL	((void*)0)				/* NULL pointer */
#endif

#define EOF (-1)

#endif /* TYPES_H_ */
