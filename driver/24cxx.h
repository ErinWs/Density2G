#ifndef _24CXX_H
#define _24CXX_H

typedef struct _24CXX_COMPONENTS
{
	char desc[32];
	struct _24CXX_COMPONENTS *this;
	unsigned char (*const write)(unsigned int addr,  void const * const buf, unsigned int n);
	unsigned char (*const read )(unsigned int addr,  void * const buf, unsigned int n);
}_24cxx_comps_t;

extern _24cxx_comps_t _24cxx_comps;
#endif