#include "render.h"
#include "Iw2D.h"
#include "IwResManager.h"
#include "IwTexture.h"

CIw2DFont *g_font;

void LoadGlobalImages()
{
	g_font = Iw2DCreateFontResource("font");
}

void UnloadGlobalImages()
{
	delete g_font;
}
