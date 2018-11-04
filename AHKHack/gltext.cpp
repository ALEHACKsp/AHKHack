#include "gltext.h"

void GLText_t::BuildFonts()
{
	fontHDC = wglGetCurrentDC();
	hackFont = glGenLists(96);
	HFONT hFont = CreateFontA(-15, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, "Consolas");
	HFONT hOldFont = (HFONT)SelectObject(fontHDC, hFont);
	wglUseFontBitmaps(fontHDC, 32, 96, hackFont);
	SelectObject(fontHDC, hOldFont);
	DeleteObject(hFont);

	bFontBuilt = true;
}

void GLText_t::Print(float x, float y, const unsigned char color[3], const char *format, ...)
{
	glColor3ub(color[0], color[1], color[2]);
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

Vector3D GLText_t::centerText(float x, float y, float width, float height, float textWidth, float textHeight)
{
	Vector3D text;
	text.x = x + (width - textWidth) / 2;
	text.y = y + textHeight;
	return text;
}