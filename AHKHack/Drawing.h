#pragma once
#ifndef DRAWING_H
#define DRAWING_H

#include <stdio.h>
#include <Windows.h>
#include <gl/GL.h>
#pragma comment(lib, "OpenGL32.lib")

namespace Drawing
{
	void DrawBox(int x, int y, int x2, int y2, float r, float g, float b, float a);

	void DrawBoxOutline(int x, int y, int x2, int y2, int lineWidth, float r, float g, float b, float a);

	void DrawLine(int x, int y, int x2, int y2, float lineWidth, float r, float g, float b, float a);

	void DrawString(int x, int y, float r, float g, float b, float a, const char *format, ...);
}

#endif