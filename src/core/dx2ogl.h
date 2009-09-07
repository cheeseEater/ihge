#ifndef _DX2OGL_H_
#define _DX2OGL_H_


#if !HGE_IPHONE

	#define GLEW_STATIC
	#include "../../include/gl/glew.h"
	
	#include <gl/gl.h>

#else
	#include <OpenGLES/EAGL.h>
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#endif



#define GL_ERROR(err) ((((err = glGetError()) != GL_NO_ERROR)) ? true : false)

typedef struct _D3DRECT {
    LONG x1;
    LONG y1;
    LONG x2;
    LONG y2;
} D3DRECT;

typedef DWORD D3DCOLOR;


struct IDirect3DTexture9
{

};



struct IDirect3DSurface9
{

};



/* Resize Optional Parameters */
typedef struct _D3DPRESENT_PARAMETERS_
{

} D3DPRESENT_PARAMETERS;


struct IDirect3D9
{

};

struct IDirect3DDevice9
{

	HRESULT Clear(  DWORD Count,
					CONST D3DRECT * pRects,
					DWORD Flags,
					D3DCOLOR Color,
					float Z,
					DWORD Stencil);
};

struct IDirect3DVertexBuffer9
{

};


struct IDirect3DIndexBuffer9
{

};


typedef struct _D3DMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };
} D3DMATRIX;


typedef struct D3DXMATRIX : public D3DMATRIX
{

} D3DXMATRIX, *LPD3DXMATRIX;


typedef enum _D3DFORMAT
{

} D3DFORMAT;



#endif //_DX2OGL_H_
