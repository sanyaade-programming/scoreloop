#ifndef _GUI_H
#define _GUI_H

#include "Iw2D.h"

class CGUIControl
{
protected:
	bool m_visible;
	CIwSVec2 m_topLeft;
	CIwSVec2 m_size;
	bool m_enabled;
	bool m_capture;
public:
	CGUIControl(CIwSVec2 _topLeft, CIwSVec2 _size){m_topLeft = _topLeft; m_size = _size; m_enabled = true; m_visible = true; m_capture = false;};
	virtual ~CGUIControl(){};

	virtual void Render(){};
	virtual void Update(int deltaTimeMs){};
	virtual bool Down(int32 x, int32 y){return false;};
	virtual bool Up(int32 x, int32 y){return false;};
	virtual bool Move(int32 x, int32 y){return false;};

	CIwSVec2 get_topLeft(){return m_topLeft;};
	CIwSVec2 get_size(){return m_size;};
	bool GetEnabled(){return m_enabled;};
	void SetEnabled(bool value){m_enabled = value;};
	bool GetVisible(){return m_visible;};
	void SetVisible(bool value){m_visible = value;};
	bool GetCapture(){return m_capture;};
	void SetCapture(bool value){m_capture = value;};
};



class CGUILabel: public CGUIControl
{
protected:
	char m_caption[64];
	int32 m_fontcolor;
	CIw2DFont *m_font;
	bool m_fontowner;

	CIw2DFontAlign m_horzalign;
	CIw2DFontAlign m_vertalign;
public:
	CGUILabel(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DFont *_font, bool _fontowner);
	~CGUILabel();

	void Render();
	char *GetCaption(){return m_caption;};
	void SetCaption(const char *value){sprintf(m_caption, value);};
	void SetCaptioni(int value){sprintf(m_caption, "%d", value);};
	void SetCaptiont(int value){ sprintf(m_caption, "%02d:%02d", value / 1000 /60, value / 1000 % 60);};
	int32 GetFontColor(){return m_fontcolor;};
	void SetFontColor(int32 value){m_fontcolor = value;};

	CIw2DFontAlign GetHorzAlign(){return m_horzalign;};
	CIw2DFontAlign GetVertAlign(){return m_vertalign;};
	void SetHorzAlign(CIw2DFontAlign value){m_horzalign = value;};
	void SetVertAlign(CIw2DFontAlign value){m_vertalign = value;};

};

class CGUIImageButton: public CGUILabel
{
protected:

	CIw2DImage *m_image;
	bool m_down;
	bool m_over;
	bool m_imageowner;
public:
	CGUIImageButton(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_image, bool _imageowner, CIw2DFont *_font, bool _fontowner);
	~CGUIImageButton();

	void Render();
	bool Down(int32 x, int32 y);
	bool Up(int32 x, int32 y);
	bool Move(int32 x, int32 y);
};

class CGUICheckButton: public CGUIImageButton
{
private:
	CIw2DImage *m_checkedimage;
	bool m_checked;
public:
	CGUICheckButton(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_image, CIw2DImage *_checkedimage, 
					bool _imageowner, CIw2DFont *_font, bool _fontowner);
	~CGUICheckButton();

	void Render();
	bool Up(int32 x, int32 y);

	bool GetChecked(){return m_checked;};
	void SetChecked(bool value){m_checked = value;};
};

class CGUIDialog: public CGUIControl 
{
protected:
	
	CIw2DImage *m_bgimage;
	CIwArray<CGUIControl *> m_guicontrols;
	int m_downcontrolindex;
	int m_modalres;
public:
	CGUIDialog(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_bgimage);
	~CGUIDialog();

	void Render();
	void Update(int deltaTimeMs);
	bool Down(int32 x, int32 y);
	bool Up(int32 x, int32 y);
	bool Move(int32 x, int32 y);

	void Show(){m_visible = true; m_downcontrolindex = -1; m_modalres = -1;};
	void Close(){m_visible = false; m_downcontrolindex = -1;};

	void AddControl(CGUIControl *_control){m_guicontrols.push_back(_control);};

	uint32 GetControlCount(){return m_guicontrols.size();};
	CGUIControl *GetControl(uint32 index){return m_guicontrols[index];};

	bool Get_Visible(){return m_visible;};
	int Get_ModalRes(){return m_modalres;};
};


class CGUIKeyboard: public CGUIControl 
{
protected:
	char m_string[64];
	CIw2DFont *m_font;
	CIw2DImage *m_btnimage;
	CIwArray<CGUIControl *> m_guicontrols;
	int32 m_time;
	int m_downcontrolindex;
	int m_modalres;
	bool m_caps;

	void ChangeCaps(bool value);
public:
	CGUIKeyboard(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_btnimage, CIw2DFont * _font);
	~CGUIKeyboard();

	void Render();
	void Update(int deltaTimeMs);
	bool Down(int32 x, int32 y);
	bool Up(int32 x, int32 y);
	bool Move(int32 x, int32 y);

	void Show(){m_visible = true; m_downcontrolindex = -1; m_modalres = -1; ChangeCaps(true);};
	void Close(){m_visible = false; m_downcontrolindex = -1;};


	bool Get_Visible(){return m_visible;};
	int Get_ModalRes(){return m_modalres;};
	char *GetString(){return m_string;};
	void SetString(char *value){sprintf(m_string, value);};
};

class CGUITable: public CGUIControl
{
protected:
	char (*m_cells)[128];
	int16 (*m_colwidths);
	CIw2DFontAlign (*m_colaligns);
	int32 m_fontcolor;
	CIw2DFont *m_font;
	bool m_fontowner;
	int m_colcount;
	int m_rowcount;
public:
	CGUITable(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DFont *_font, bool _fontowner);
	~CGUITable();

	void Render();
	void SetTableSize(int _colcount, int _rowcount);
	void Clear(){if(m_cells) memset(m_cells, 0, m_colcount * m_rowcount * 128);};

	int32 GetFontColor(){return m_fontcolor;};
	void SetFontColor(int32 value){m_fontcolor = value;};
	char *GetCellText(int _col, int _row){return m_cells[_col + _row * m_colcount];};
	void SetCellText(int _col, int _row, char *value){sprintf(m_cells[_col + _row * m_colcount], value);};
	void SetCellTexti(int _col, int _row, int value){sprintf(m_cells[_col + _row * m_colcount], "%d", value);};

	int16 GetColWidth(int _col){return m_colwidths[_col];};
	void SetColWidth(int _col, int value){m_colwidths[_col] = value;};

	CIw2DFontAlign GetColAlign(int _col){return m_colaligns[_col];};
	void SetColAlign(int _col, CIw2DFontAlign value){m_colaligns[_col] = value;};
};

struct CGUIListItem
{
	char captionleft[128];
	char captionright[128];
	char subtext[128];
	CIw2DImage *image;
	bool visible;
	void *data;
	int32 fontcolor;

};

enum GUIDirection {dVertical, dHorizontal};

class CGUIListView: public CGUIControl
{
protected:
	CIwArray<CGUIListItem *> m_items;
	CIw2DFont *m_font;
	int32 m_fontcolor;
	bool m_fontowner;
	int16 m_itemsize;
	int16 m_itemsubtextsize;
	bool m_down;
	bool m_over;
	int16 m_offsetymin;
	int16 m_offsetxmin;
	int16 m_offsetx;
	int16 m_offsety;
	int16 m_offsetx0;
	int16 m_offsety0;
	int16 m_offsetprev;
	int32 m_downx;
	int32 m_downy;
	int32 m_itemindex;
	int16 m_visibleitemscount;
	int16 m_scrollspeed;
	GUIDirection m_direction;
	
	bool m_needselect;
	bool m_needimages;
	void RenderVertical();
	void RenderHorizontal();
	void UpdateVertical(int deltaTimeMs);
	void UpdateHorizontal(int deltaTimeMs);
public:
	CGUIListView(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DFont *_font, bool _fontowner);
	~CGUIListView();

	void Render();
	void Update(int deltaTimeMs);
	int AddItem(const char *resource, const char *caption);
	void SetItemCaptionLeft(int index, const char *value){sprintf(m_items[index]->captionleft, value);};
	void SetItemCaptionLefti(int index, int value){sprintf(m_items[index]->captionleft, "%d", value);};
	void SetItemCaptionRight(int index, const char *value){sprintf(m_items[index]->captionright, value);};
	void SetItemCaptionRighti(int index, int value){sprintf(m_items[index]->captionright, "%d", value);};
	void SetItemSubtext(int index, const char *value){sprintf(m_items[index]->subtext, value);};
	void SetItemSubtexti(int index, int value){sprintf(m_items[index]->subtext, "%d", value);};
	void SetItemVisible(int index, bool value){m_items[index]->visible = value; UpdateVisibleItemsCount();};
	void Clear();

	bool Down(int32 x, int32 y);
	bool Up(int32 x, int32 y);
	bool Move(int32 x, int32 y);

	void MoveItemToCenter(int index);

	void UpdateVisibleItemsCount();

	int16 GetAllItemsSize(){return m_itemsize * m_visibleitemscount;};
	int32 get_itemindex(){return m_itemindex;};
	void set_itemindex(int32 value){m_itemindex = value;};
	CGUIListItem *get_item(int index){return m_items[index];};
	uint32 get_itemscount(){return m_items.size();};
	bool get_needimages(){return m_needimages;};
	void set_needimages(bool value){m_needimages = value;};
	bool get_needselect(){return m_needselect;};
	void set_needselect(bool value){m_needselect = value;};
	int16 get_itemsize(){return m_itemsize;};
	void set_itemsize(int16 value){m_itemsize = value;};
	int16 get_itemsubtextsize(){return m_itemsubtextsize;};
	void set_itemsubtextsize(int16 value){m_itemsubtextsize = value;};
	GUIDirection get_direction(){return m_direction;};
	void set_direction(GUIDirection value){m_direction = value;};
};

#endif //_GUI_H