/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** Core functions implementation: graphics
*/


#include "hge_impl.h"

#include "soil/soil.h"


void CALL HGE_Impl::Gfx_Clear(DWORD color)
{
	//TODO: set given color
    glClearColor((float)GETR(color)/0xff, (float)GETG(color)/0xff, (float)GETB(color)/0xff, (float)GETA(color)/0xff);

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
}

void CALL HGE_Impl::Gfx_SetClipping(int x, int y, int w, int h)
{
	//TODO: insert code here
}

void CALL HGE_Impl::Gfx_SetTransform(float x, float y, float dx, float dy, float rot, float hscale, float vscale)
{
	//TODO: insert code here
}

bool CALL HGE_Impl::Gfx_BeginScene(HTARGET targ)
{
	int width	=	nScreenWidth;
	int height	=	nScreenHeight;
	pCurTarget	=	(CRenderTargetList*)targ;


//    glPolygonMode(GL_FRONT, GL_LINE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDepthMask(FALSE);
			

	//if rendering to texture use texture width and height
	if (pCurTarget)
	{
		width	= pCurTarget->target->GetWidth();
		height	= pCurTarget->target->GetHeight();
	}


	glViewport(0, 0, width, height);
	//TODO: insert code here
	glMatrixMode(GL_PROJECTION);
	
	//Clearing the projection matrix...
	glLoadIdentity();
	
	//Creating an orthoscopic view matrix going from -1 -> 1 in each
	//dimension on the screen (x, y, z). 
	glOrtho(0, width, height, 0, -1., 1.);
	

	glMatrixMode(GL_MODELVIEW);
	//Clearing model view matrix
	glLoadIdentity();


	if (pCurTarget)
		pCurTarget->target->AttachToRender();
	
	return true;
}

void CALL HGE_Impl::Gfx_EndScene()
{
	glFlush();
	if (pCurTarget)
	{
		pCurTarget->target->DetachFromRender();
	} 
	else
		SwapBuffers(GetDC(hwnd));
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CALL HGE_Impl::Gfx_RenderLine(float x1, float y1, float x2, float y2, DWORD color, float z)
{
	//TODO: insert code here
}

void CALL HGE_Impl::Gfx_RenderTriple(const hgeTriple *triple)
{
	//TODO: insert code here
}

void CALL HGE_Impl::Gfx_RenderQuad(const hgeQuad *quad)
{

	glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D, quad->tex);

	_SetBlendMode(quad->blend);


	//TODO: insert code here
	GLfloat verteces[12] = {
		quad->v[3].x, quad->v[3].y, quad->v[3].z,
		quad->v[2].x, quad->v[2].y, quad->v[2].z,
		quad->v[1].x, quad->v[1].y, quad->v[1].z,
		quad->v[0].x, quad->v[0].y, quad->v[0].z,
	};

	GLfloat texCoords[8] = {
		quad->v[3].tx, quad->v[3].ty,
		quad->v[2].tx, quad->v[2].ty,
		quad->v[1].tx, quad->v[1].ty,
		quad->v[0].tx, quad->v[0].ty,
	};
	GLubyte colors[16] = {
		(GLubyte)GETR(quad->v[3].col), (GLubyte)GETG(quad->v[3].col), (GLubyte)GETB(quad->v[3].col), (GLubyte)GETA(quad->v[3].col),
		(GLubyte)GETR(quad->v[2].col), (GLubyte)GETG(quad->v[2].col), (GLubyte)GETB(quad->v[2].col), (GLubyte)GETA(quad->v[2].col),
		(GLubyte)GETR(quad->v[1].col), (GLubyte)GETG(quad->v[1].col), (GLubyte)GETB(quad->v[1].col), (GLubyte)GETA(quad->v[1].col),
		(GLubyte)GETR(quad->v[0].col), (GLubyte)GETG(quad->v[0].col), (GLubyte)GETB(quad->v[0].col), (GLubyte)GETA(quad->v[0].col),

	};


	glVertexPointer(3, GL_FLOAT, 0, verteces);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_TEXTURE_2D); // Enable Texture Mapping
	glDisable(GL_BLEND); // Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D, 0);


}

hgeVertex* CALL HGE_Impl::Gfx_StartBatch(int prim_type, HTEXTURE tex, int blend, int *max_prim)
{
	//TODO: insert code here
	return 0;
}

void CALL HGE_Impl::Gfx_FinishBatch(int nprim)
{
	//TODO: insert code here
}

HTARGET CALL HGE_Impl::Target_Create(int width, int height, bool zbuffer)
{
	CRenderTargetList *pTarget;
	
	pTarget = new CRenderTargetList;
	pTarget->target = CGfxTarget::CreateTarget(width, height, zbuffer);

	if (!(pTarget->target))
	{
		delete pTarget;
		return 0;
	}

	pTarget->next=pTargets;
	pTargets=pTarget;

	return (HTARGET)pTarget;
}

void CALL HGE_Impl::Target_Free(HTARGET target)
{
	CRenderTargetList *pTarget=pTargets, *pPrevTarget=NULL;

	while(pTarget)
	{
		if((CRenderTargetList *)target == pTarget)
		{
			if(pPrevTarget)
				pPrevTarget->next = pTarget->next;
			else
				pTargets = pTarget->next;

			if(pTarget->target) 
			{
				delete (pTarget->target);
			}

			delete pTarget;
			return;
		}

		pPrevTarget = pTarget;
		pTarget = pTarget->next;
	}
	

}

HTEXTURE CALL HGE_Impl::Target_GetTexture(HTARGET target_)
{
	CRenderTargetList *targ=(CRenderTargetList *)target_;
	if(target_) return targ->target->GetTexture();
	else return 0;

	return 0;
}

HTEXTURE CALL HGE_Impl::Texture_Create(int width, int height)
{
	//TODO: insert code here
	return 0;
}

HTEXTURE CALL HGE_Impl::Texture_Load(const char *filename, DWORD size, bool bMipmap)
{
	void *data;
	DWORD _size;
	HTEXTURE pTex;
	CTextureList *texItem;
	int w, h, channels;

	if(size)
	{ 
		data=(void *)filename; _size=size; 
	}
	else
	{
		data=pHGE->Resource_Load(filename, &_size);
		if(!data) return NULL;
	}

	
	pTex = SOIL_load_OGL_texture_and_info_from_memory(
								(const unsigned char* const)data,
								_size, 
								SOIL_LOAD_AUTO,
								SOIL_CREATE_NEW_ID,
								(bMipmap ? SOIL_FLAG_MIPMAPS : 0) 
								| SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_TEXTURE_REPEATS,
								&w, &h, &channels);
	
	if (0 == pTex)	
	{
		_PostError("Can't create texture");
		if(!size) Resource_Free(data);
		return NULL;
	}

	if(!size) Resource_Free(data);
	
	texItem=new CTextureList;
	texItem->tex=(HTEXTURE)pTex;
	texItem->next=textures;
	textures=texItem;
	
	return (HTEXTURE)pTex;
}

void CALL HGE_Impl::Texture_Free(HTEXTURE tex)
{
	CTextureList *texItem=textures, *texPrev=0;

	while(texItem)
	{
		if(texItem->tex==tex)
		{
			if(texPrev) texPrev->next=texItem->next;
			else textures=texItem->next;
			delete texItem;
			break;
		}
		texPrev=texItem;
		texItem=texItem->next;
	}
	glDeleteTextures(1, (const GLuint*)&tex);
}

int CALL HGE_Impl::Texture_GetWidth(HTEXTURE tex, bool bOriginal)
{
	GLint w; 
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
	glBindTexture(GL_TEXTURE_2D, 0);
	return w;
}


int CALL HGE_Impl::Texture_GetHeight(HTEXTURE tex, bool bOriginal)
{
	GLint h; 
	glBindTexture(GL_TEXTURE_2D, tex);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
	glBindTexture(GL_TEXTURE_2D, 0);
	return h;
}

DWORD * CALL HGE_Impl::Texture_Lock(HTEXTURE tex, bool bReadOnly, int left, int top, int width, int height)
{
	//TODO:
/*	GLvoid *image;
	CTextureList *texItem=textures;
	GLint fmt;

	while(texItem && (texItem->tex!=tex))
	{
		texItem=texItem->next;
	}
	
	if (!texItem)
		return 0;
	
	RECT region;
	int flags;


	if(!width || !height)
	{
	  return 0;
	}
	
	glBindTexture(GL_TEXTURE_2D, tex);


	glGetTexLevelParameter(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);

	if (fmt != GL_RGBA) return 0;

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, image);

	if(FAILED(pTex->LockRect(0, &TRect, prec, flags)))
	{
		_PostError("Can't lock texture");
		return 0;
	}

	return (DWORD *)image;*/
	return 0;

}


void CALL HGE_Impl::Texture_Unlock(HTEXTURE tex)
{
	//TODO: insert code here
}

//////// Implementation ////////

void HGE_Impl::_render_batch(bool bEndScene)
{
	//TODO: insert code here
}

void HGE_Impl::_SetBlendMode(int blend)
{

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND); // Enable Blending


	if((blend & BLEND_ALPHABLEND) != (CurBlendMode & BLEND_ALPHABLEND))
	{
		if(blend & BLEND_ALPHABLEND) 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Alpha blending
		else 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE); //Addictive	
	}

    if((blend & BLEND_ZWRITE) != (CurBlendMode & BLEND_ZWRITE))
	{
		if(blend & BLEND_ZWRITE) glDepthMask(GL_TRUE);
		else glDepthMask(GL_FALSE);
	}	
	
	if((blend & BLEND_COLORADD) != (CurBlendMode & BLEND_COLORADD))
	{
		if(blend & BLEND_COLORADD) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD); 
		else glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
	}
	
	CurBlendMode = blend;
}

void HGE_Impl::_SetProjectionMatrix(int width, int height)
{
	//TODO: insert code here

	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);
	
	//Clearing the projection matrix...
	glLoadIdentity();
	
	//Creating an orthoscopic view matrix going from -1 -> 1 in each
	//dimension on the screen (x, y, z). 
	glOrtho(0, nScreenWidth, nScreenHeight, 0, -1., 1.);

}

bool HGE_Impl::_GfxInit()
{
//	static const char *szFormats[]={"UNKNOWN", "R5G6B5", "X1R5G5B5", "A1R5G5B5", "X8R8G8B8", "A8R8G8B8"};
//	D3DADAPTER_IDENTIFIER9 AdID;
//	D3DDISPLAYMODE Mode;
//	D3DFORMAT Format=D3DFMT_UNKNOWN;
//	UINT nModes, i;
	
// Init Open GL
							
	HDC hDC;
	HGLRC hRC;
	long gl_error;

	
	hDC = GetDC(pHGE->hwnd);
	hRC = wglCreateContext(hDC);

	if (hRC == NULL)
	{
		System_Log("Creating GL Context Failed with error code: %d", GetLastError());
		return false;
	}
	
	System_Log("Initializing Graphix");

	
	
	wglMakeCurrent(hDC, hRC);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
		System_Log("GLEW init Error: %s\n", glewGetErrorString(err));
	} 
	else
		System_Log("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));	

	if (!GLEW_EXT_framebuffer_object)
	{
		System_Log("Status: Unavailable Extension GL_EXT_framebuffer_object\n");	
	}
	else
		System_Log("Status: GL_EXT_framebuffer_object is supportet\n");	

	

	glShadeModel(GL_SMOOTH);						// Enables Smooth Shading
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	
    glClearStencil(0);                          // clear stencil buffer
	glClearDepth(1.0f);								// Depth Buffer Setup
	

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
//	glDisable(GL_DEPTH_TEST);
	

	glDepthFunc(GL_LEQUAL);							// The Type Of Depth Test To Do

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations



// Get adapter info

	System_Log("OpenGL version: %s", glGetString( GL_VERSION ));
//	System_Log("OpenGL version: %s", glGetString( GL_EXTENSIONS ));

	


// Set up Full Screen presentation parameters


	
	_AdjustWindow();

	System_Log("Mode: width = %d height = %d\n",nScreenWidth,nScreenHeight);

// Create vertex batch buffer

//	VertArray=0;
	textures=0;

// Init all stuff that can be lost

	_SetProjectionMatrix(nScreenWidth, nScreenHeight);

	if (GL_ERROR(gl_error))
	{
		System_Log("OpenGL error: %d", gl_error);
		return false;
	}

	if(!_init_lost()) return false;

	Gfx_Clear(0);

	System_Log("Graphix initialized");

	return true;
}

int HGE_Impl::_format_id(D3DFORMAT fmt)
{
	//TODO: insert code here
	return 0;
}

void HGE_Impl::_AdjustWindow()
{
	//TODO: insert code here
	RECT *rc;
	LONG style;

	if(bWindowed) {rc=&rectW; style=styleW; }
	else  {rc=&rectFS; style=styleFS; }
	SetWindowLong(hwnd, GWL_STYLE, style);

	style=GetWindowLong(hwnd, GWL_EXSTYLE);
	if(bWindowed)
	{
		SetWindowLong(hwnd, GWL_EXSTYLE, style & (~WS_EX_TOPMOST));
	    SetWindowPos(hwnd, HWND_NOTOPMOST, rc->left, rc->top, rc->right-rc->left, rc->bottom-rc->top, SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_TOPMOST);
	    SetWindowPos(hwnd, HWND_TOPMOST, rc->left, rc->top, rc->right-rc->left, rc->bottom-rc->top, SWP_FRAMECHANGED);
	}

}

void HGE_Impl::_Resize(int width, int height)
{
	//TODO: insert code here
}

void HGE_Impl::_GfxDone()
{
	//TODO: insert code here
}


bool HGE_Impl::_GfxRestore()
{
	//TODO: insert code here
	return false;
}


bool HGE_Impl::_init_lost()
{
	CRenderTargetList *target=pTargets;

// Store render target

	
// Set common render states

	//pD3DDevice->SetRenderState( D3DRS_LASTPIXEL, FALSE );
//	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);


	nPrim=0;
	CurPrimType=HGEPRIM_QUADS;
	CurBlendMode = BLEND_DEFAULT;
	CurTexture = NULL;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);	

	return true;
}
