#include "Drawing.h"

namespace Drawing
{
	HDC font;
	unsigned int hackFont;

	void DrawBox(int x, int y, int x2, int y2, float r, float g, float b, float a)
	{
		glDepthMask(GL_FALSE);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(r, g, b, a);

		glBegin(GL_QUADS);

		glVertex2f(x, y);
		glVertex2f(x2, y);
		glVertex2f(x2, y2);
		glVertex2f(x, y2);

		glEnd();
	}

	void DrawBoxOutline(int x, int y, int x2, int y2, int lineWidth, float r, float g, float b, float a)
	{
		glDepthMask(GL_FALSE);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLfloat glTemp[1];
		glGetFloatv(GL_LINE_WIDTH, glTemp);

		glLineWidth(lineWidth);
		glColor4f(r, g, b, a);

		glBegin(GL_LINE_LOOP);

		glVertex2f(x, y);
		glVertex2f(x2, y);
		glVertex2f(x2, y2);
		glVertex2f(x, y2);

		glEnd();

		glLineWidth(glTemp[0]);
	}

	void DrawLine(int x, int y, int x2, int y2, float lineWidth, float r, float g, float b, float a)
	{
		glDepthMask(GL_FALSE);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		GLfloat glTemp[1];
		glGetFloatv(GL_LINE_WIDTH, glTemp);

		glLineWidth(lineWidth);
		glColor4f(r, g, b, a);

		glBegin(GL_LINES);

		glVertex2f(x, y);
		glVertex2f(x2, y2);

		glEnd();

		glLineWidth(glTemp[0]);
	}

	void DrawString(int x, int y, float r, float g, float b, float a, const char *format, ...)
	{
		HDC fontHDC = wglGetCurrentDC();

		if (font != fontHDC)
		{
			hackFont = glGenLists(96);
			HFONT hFont = CreateFontA(-15, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Arial");
			HFONT hOldFont = (HFONT)SelectObject(fontHDC, hFont);
			wglUseFontBitmaps(fontHDC, 32, 96, hackFont);
			SelectObject(fontHDC, hOldFont);
			DeleteObject(hFont);
			font = fontHDC;
		}

		glDepthMask(GL_FALSE);
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(r, g, b, a);
		glRasterPos2f(x, y);

		char text[100];
		va_list	args;

		va_start(args, format);
		vsprintf_s(text, 100, format, args);
		va_end(args);

		glPushAttrib(GL_LIST_BIT);
		glListBase(hackFont - 32);
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
		glPopAttrib();
	}
}