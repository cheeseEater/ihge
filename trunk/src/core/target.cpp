/*
** HGEPort 0.1
** Copyright (C) 2009, Alexander Katasonov
**
*/


#include "target.h"

#include "dx2ogl.h"

class CGfxTargetFrameBufferOG : public CGfxTarget
{
	DWORD			frameBuffer;
	DWORD			depthBuffer;

public:
				CGfxTargetFrameBufferOG(DWORD w, DWORD h, bool zbuffer);
	virtual		~CGfxTargetFrameBufferOG();

	virtual void	AttachToRender();
	virtual void	DetachFromRender();

};



CGfxTargetFrameBufferOG::CGfxTargetFrameBufferOG(DWORD w_, DWORD h_, bool zbuffer_)
	: CGfxTarget(w_, h_, zbuffer_)
{
	long gl_error;

	frameBuffer = 0;
	depthBuffer = 0;


	if (!GLEW_EXT_framebuffer_object)
	{
		throw 1;
	}


	
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, (unsigned int*)(&texture));					// Create 1 Texture
	glBindTexture(GL_TEXTURE_2D, texture);			// Bind The Texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap generation included in OpenGL v1.4
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);			// Bind The Texture

	if (GL_ERROR(gl_error))
	{
		throw 1;
	}
	
	glDisable(GL_TEXTURE_2D);

   // create framebuffer
	glGenFramebuffersEXT(1, (unsigned int*)(&frameBuffer));
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);

	//create render buffer
	if (zbuffer)
	{
		glGenRenderbuffersEXT(1, (unsigned int*)(&depthBuffer));
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}

	// attach texture
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
	// Attach to the FBO for depth
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer); 

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	if (!texture || !frameBuffer)
		throw 1;
}

CGfxTargetFrameBufferOG::~CGfxTargetFrameBufferOG()
{
	if (texture)
		glDeleteTextures(1, (unsigned int*)(&texture));
	
	if (frameBuffer)
		glDeleteFramebuffersEXT(1, (unsigned int*)(&frameBuffer));
	
	if (depthBuffer)
		glDeleteRenderbuffersEXT(1, (unsigned int*)(&depthBuffer));
}

void CGfxTargetFrameBufferOG::AttachToRender()
{
	// set the rendering destination to FBO
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
}

void CGfxTargetFrameBufferOG::DetachFromRender()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}



CGfxTarget* CGfxTarget::CreateTarget(int w, int h, bool zbuffer)
{
	try {
		return (CGfxTarget*)(new CGfxTargetFrameBufferOG(w, h, zbuffer));
	}
	catch(...)
	{
		return 0;
	}

}
