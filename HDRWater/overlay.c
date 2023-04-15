#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

int blend_src, blend_dst;

void BeginOverlay(int Width, int Height)
{
	glGetIntegerv(GL_BLEND_SRC, &blend_src);
	glGetIntegerv(GL_BLEND_DST, &blend_dst);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0f, Width, 0.0f, Height);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void EndOverlay(void)
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDepthMask(GL_TRUE);

	glPopAttrib();

	glBlendFunc(blend_src, blend_dst);
}