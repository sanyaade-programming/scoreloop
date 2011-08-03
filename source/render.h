
#ifndef _RENDERING_H
#define _RENDERING_H



// Foward declarations

class CIw2DFont;

void LoadGlobalImages();
void UnloadGlobalImages();

// Global variable used by the rendering functions to scale graphics.
extern CIw2DFont *g_font;


#endif // _RENDERING_H