#include "gui.h"
#include "IwGx.h"
#include <cctype>

CGUIImageButton::CGUIImageButton(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_image, bool _imageowner, CIw2DFont *_font, bool _fontowner): 
CGUILabel(_topLeft, _size, _font, _fontowner)
{
	m_image = _image;
	m_down = false;
	m_over = false;
	m_imageowner = _imageowner;
	m_horzalign = IW_2D_FONT_ALIGN_CENTRE;
}

CGUIImageButton::~CGUIImageButton()
{
	if (m_imageowner && m_image)
		delete m_image;
}


void CGUIImageButton::Render()
{
	if (!m_visible)
		return;
	CIwMat2D prevtm = Iw2DGetTransformMatrix();
	CIwMat2D tm;
	tm.SetIdentity();

	tm.SetTrans(prevtm.t + m_topLeft + m_size * IW_FIXED(0.5));
	Iw2DSetTransformMatrix(tm);

		if (m_enabled)
		{
			if (m_down && (m_over || m_capture))
				Iw2DSetColour(0xffcccccc);
			else
				Iw2DSetColour(0xffffffff);
		}
		else
			Iw2DSetColour(0x66ffffff);
		if (m_image)
			Iw2DDrawImage(m_image, m_size * IW_FIXED(-0.5), m_size);
		else
			Iw2DFillRect(m_size * IW_FIXED(-0.5), m_size);
	if (m_font && m_caption[0])
	{
		CIwColour clr = Iw2DGetColour();
		if (m_enabled)
			Iw2DSetColour(m_fontcolor);
		else
			Iw2DSetColour(0xff999999);
		
		Iw2DSetFont(m_font);
		Iw2DDrawString(m_caption, m_size * IW_FIXED(-0.5), m_size, m_horzalign, m_vertalign);
		Iw2DSetColour(clr);
	}
	Iw2DSetTransformMatrix(prevtm);

}

bool CGUIImageButton::Down(int32 x, int32 y)
{
	bool res = (m_enabled &&
		        m_visible &&
		        x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	m_down = res;
	m_over = res;
	return res;
}

bool CGUIImageButton::Up(int32 x, int32 y)
{
	bool res = (m_enabled &&
				m_visible &&
				x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	m_down = false;
	m_over = false;
	return res;
}

bool CGUIImageButton::Move(int32 x, int32 y)
{
	m_over =   (m_enabled &&
				m_visible &&
				x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	return m_over;
}


CGUICheckButton::CGUICheckButton(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_image, CIw2DImage *_checkedimage, 
								 bool _imageowner, CIw2DFont *_font, bool _fontowner):
								CGUIImageButton(_topLeft, _size, _image, _imageowner, _font, _fontowner)	
{
	m_checkedimage = _checkedimage;
	m_checked = false;
}

CGUICheckButton::~CGUICheckButton()
{
	if (m_imageowner && m_checkedimage)
		delete m_checkedimage;
}

void CGUICheckButton::Render()
{
	if (!m_visible)
		return;
	CIwMat2D prevtm = Iw2DGetTransformMatrix();
	CIwMat2D tm;
	tm.SetIdentity();

	tm.SetTrans(prevtm.t + m_topLeft + m_size * IW_FIXED(0.5));
	Iw2DSetTransformMatrix(tm);
	if (m_image)
	{
		if (m_enabled)
		{
			if (m_down && (m_over || m_capture))
				Iw2DSetColour(0xffcccccc);
			else
				Iw2DSetColour(0xffffffff);
		}
		else
			Iw2DSetColour(0x66ffffff);
		if (m_checked)
			Iw2DDrawImage(m_checkedimage, m_size * IW_FIXED(-0.5), m_size);
		else
			Iw2DDrawImage(m_image, m_size * IW_FIXED(-0.5), m_size);
	}
	if (m_font && m_caption[0])
	{
		CIwColour clr = Iw2DGetColour();
		if (m_enabled)
			Iw2DSetColour(m_fontcolor);
		else
			Iw2DSetColour(0xff999999);
		
		Iw2DSetFont(m_font);
		Iw2DDrawString(m_caption, m_size * IW_FIXED(-0.5), m_size, m_horzalign, m_vertalign);
		Iw2DSetColour(clr);
	}
	Iw2DSetTransformMatrix(prevtm);

}


bool CGUICheckButton::Up(int32 x, int32 y)
{
	bool res = (m_enabled &&
				m_visible &&
				x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	m_down = false;
	m_over = false;
	if (res)
		m_checked = !m_checked;
	return res;
}

CGUILabel::CGUILabel(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DFont *_font, bool _fontowner): CGUIControl(_topLeft, _size)
{
	m_font = _font;
	m_fontowner = _fontowner;
	m_fontcolor = 0xff000000;
	m_horzalign = IW_2D_FONT_ALIGN_LEFT;
	m_vertalign = IW_2D_FONT_ALIGN_CENTRE;
	memset(m_caption, 0, 64);
}

CGUILabel::~CGUILabel()
{
	if (m_fontowner && m_font) 
		delete m_font;
}


void CGUILabel::Render()
{
	if (!m_visible)
		return;
	if (m_font)
	{
		//Iw2DSetColour(0x55ff0000);
		//Iw2DFillRect(m_topLeft, m_size);
		CIwColour clr = Iw2DGetColour();
		Iw2DSetColour(m_fontcolor);
		Iw2DSetFont(m_font);
		Iw2DDrawString(m_caption, m_topLeft, m_size, m_horzalign, m_vertalign);
		Iw2DSetColour(clr);
	}
}


CGUIDialog::CGUIDialog(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_bgimage): CGUIControl(_topLeft, _size)
{
	m_bgimage = _bgimage;
	m_downcontrolindex = -1;
	m_visible = false;
}
CGUIDialog::~CGUIDialog()
{
	if (m_bgimage)
		delete m_bgimage;
	for (uint32 i = 0; i < m_guicontrols.size(); i++)
		delete m_guicontrols[i];
	m_guicontrols.clear();
}

void CGUIDialog::Render()
{
	if (!m_visible)
		return;
	CIwMat2D tm;
	tm.SetIdentity();
	tm.SetTrans(m_topLeft);
	Iw2DSetTransformMatrix(tm);

	if (m_bgimage)
		Iw2DDrawImage(m_bgimage, CIwSVec2(0, 0), m_size);
	else
	{
		Iw2DSetColour(0xffffffff);
		Iw2DFillRect(CIwSVec2(0, 0), m_size);
	}
	for (uint32 i = 0; i < m_guicontrols.size(); i++)
		m_guicontrols[i]->Render();
	Iw2DSetTransformMatrix(CIwMat2D::g_Identity);

}

void CGUIDialog::Update(int deltaTimeMs)
{
	if (!m_visible)
		return;
	for (uint32 i = 0; i < m_guicontrols.size(); i++)
		m_guicontrols[i]->Update(deltaTimeMs);
}

bool CGUIDialog::Down(int32 x, int32 y)
{
	if (!m_visible)
		return false;
	x -= get_topLeft().x;
	y -= get_topLeft().y;
	m_downcontrolindex = -1;
	for (int32 i = m_guicontrols.size() - 1; m_downcontrolindex == -1 && i >= 0; i--)
	{
		if (m_guicontrols[i]->Down(x, y))
			m_downcontrolindex = i;
	}
	return m_downcontrolindex >= 0;
}
bool CGUIDialog::Up(int32 x, int32 y)
{
	if (!m_visible)
		return false;
	x -= get_topLeft().x;
	y -= get_topLeft().y;
	bool res = (m_downcontrolindex >= 0 && m_guicontrols[m_downcontrolindex]->Up(x, y));
	if (res)
	{
		m_modalres = m_downcontrolindex;
	}
	m_downcontrolindex = -1;
	return res;
}
bool CGUIDialog::Move(int32 x, int32 y)
{
	if (!m_visible)
		return false;
	x -= get_topLeft().x;
	y -= get_topLeft().y;
	return (m_downcontrolindex >= 0 && m_guicontrols[m_downcontrolindex]->Move(x, y));

}

CGUIKeyboard::CGUIKeyboard(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DImage *_btnimage, CIw2DFont * _font): CGUIControl(_topLeft, _size)
{
	m_font = _font;
	m_btnimage = _btnimage;
	m_downcontrolindex = -1;
	m_time = 0;
	m_visible = false;
	memset(m_string, 0, 32);

	//Filling keyboard
	m_caps = true;
	CGUIImageButton *btn;
	int16 btnh = (_size.y - 2 * 8) / 7;
	int16 btnw = 0;
	int16 l, t;
	//First line
	btnw = (_size.x - 2 * 8) / 7;
	l = 2;
	t = 2 + btnh + 2;

	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption(".");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("!");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("@");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("-");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("_");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("$");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("*");
	m_guicontrols.push_back(btn);

	//Second Line
	btnw = (_size.x - 2 * 11) / 10;
	l = 2;
	t += btnh + 2;

	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("1");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("2");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("3");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("4");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("5");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("6");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("7");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("8");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("9");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("0");
	m_guicontrols.push_back(btn);

	//Third line
	btnw = (_size.x - 2 * 11) / 10;
	l = 2;
	t += btnh + 2;

	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("Q");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("W");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("E");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("R");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("T");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("Y");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("U");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("I");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("O");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("P");
	m_guicontrols.push_back(btn);

	//Fourth line
	btnw = (_size.x - 2 * 10) / 9;
	l = 2;
	t += btnh + 2;

	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("A");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("S");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("D");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("F");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("G");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("H");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("J");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("K");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("L");
	m_guicontrols.push_back(btn);

	//Fifth line
	btnw = (_size.x - 2 * 8) / 7;
	l = 2;
	t += btnh + 2;

	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("Z");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("X");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("C");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("V");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("B");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("N");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("M");
	m_guicontrols.push_back(btn);


	//Sixth line
	btnw = (_size.x - 2 * 7) / 6;
	l = 2;
	t += btnh + 2;

	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("CAPS");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("DEL");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw * 2, btnh), 0, false, m_font, false);
	btn->SetCaption(" ");
	m_guicontrols.push_back(btn);
	l += btnw * 2 + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("OK");
	m_guicontrols.push_back(btn);
	l += btnw + 2;
	btn = new CGUIImageButton(CIwSVec2(l, t), CIwSVec2(btnw, btnh), 0, false, m_font, false);
	btn->SetCaption("CANCEL");
	m_guicontrols.push_back(btn);
}

CGUIKeyboard::~CGUIKeyboard()
{
	for (uint32 i = 0; i < m_guicontrols.size(); i++)
		delete m_guicontrols[i];
	m_guicontrols.clear();
	if (m_btnimage)
		delete m_btnimage;
}

void CGUIKeyboard::Render()
{
	if (!m_visible)
		return;


	CIwMat2D tm;
	tm.SetIdentity();
	tm.SetTrans(m_topLeft);
	Iw2DSetTransformMatrix(tm);

	Iw2DSetColour(0x99999999);
	Iw2DFillRect(CIwSVec2(0, 0), m_size);
	Iw2DSetColour(0xff000000);
	Iw2DDrawString(m_string, CIwSVec2(0, 0), CIwSVec2(m_size.x, m_size.y / 7), IW_2D_FONT_ALIGN_CENTRE, IW_2D_FONT_ALIGN_CENTRE);
	if (m_time % 2000 < 1000)
	{
		int16 strw = Iw2DGetStringWidth(m_string);
		int16 lw = Iw2DGetStringWidth("A");		
		int16 lh = (int16)m_font->GetHeight();
		Iw2DFillRect(CIwSVec2((m_size.x + strw) / 2, (m_size.y / 7 - lh) / 2), CIwSVec2(lw, lh));
	}

	for (uint32 i = 0; i < m_guicontrols.size(); i++)
		m_guicontrols[i]->Render();
	Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
}

void CGUIKeyboard::Update(int deltaTimeMs)
{
	if (!m_visible)
		return;
	m_time += deltaTimeMs;
	for (uint32 i = 0; i < m_guicontrols.size(); i++)
		m_guicontrols[i]->Update(deltaTimeMs);
}

void CGUIKeyboard::ChangeCaps(bool value)
{
	if (m_caps != value)
	{
		char buf[2];
		buf[1] = 0;
		m_caps = value;
		for (uint32 i = 17; i < m_guicontrols.size() - 5; i++)
		{
			if (m_caps)
				buf[0] = toupper(*((CGUIImageButton *)m_guicontrols[i])->GetCaption());
			else
				buf[0] =tolower(*((CGUIImageButton *)m_guicontrols[i])->GetCaption());
			((CGUIImageButton *)m_guicontrols[i])->SetCaption(buf);
		}
	}
}

bool CGUIKeyboard::Down(int32 x, int32 y)
{
	if (!m_visible)
		return false;
	x -= get_topLeft().x;
	y -= get_topLeft().y;
	m_downcontrolindex = -1;
	for (int32 i = m_guicontrols.size() - 1; m_downcontrolindex == -1 && i >= 0; i--)
	{
		if (m_guicontrols[i]->Down(x, y))
			m_downcontrolindex = i;
	}
	return m_downcontrolindex >= 0;
}

bool CGUIKeyboard::Up(int32 x, int32 y)
{
	if (!m_visible)
		return false;
	x -= get_topLeft().x;
	y -= get_topLeft().y;
	bool res = (m_downcontrolindex >= 0 && m_guicontrols[m_downcontrolindex]->Up(x, y));
	if (res)
	{
		m_modalres = 0;
		if (m_downcontrolindex == (int)m_guicontrols.size() - 1) //CANCEL
		{
			m_modalres = -1;
		}
		else if (m_downcontrolindex == (int)m_guicontrols.size() - 2) //OK
		{
			m_modalres = 1;
		}
		else if (m_downcontrolindex == (int)m_guicontrols.size() - 4) //DEL
		{
			int firstzero;
			for(firstzero = 0; m_string[firstzero]; firstzero++);
			if (firstzero > 0)
				m_string[firstzero - 1] = 0;
		}
		else if (m_downcontrolindex == (int)m_guicontrols.size() - 5) //CAPS
		{
			ChangeCaps(!m_caps);
		}
		else
		{
			sprintf(m_string, "%s%s", m_string, ((CGUIImageButton *)m_guicontrols[m_downcontrolindex])->GetCaption());
			//SetString(((CGUIImageButton *)m_guicontrols[m_downcontrolindex])->GetCaption());
		}
	}
	m_downcontrolindex = -1;
	return res;
}

bool CGUIKeyboard::Move(int32 x, int32 y)
{
	if (!m_visible)
		return false;
	x -= get_topLeft().x;
	y -= get_topLeft().y;
	return (m_downcontrolindex >= 0 && m_guicontrols[m_downcontrolindex]->Move(x, y));

}

CGUITable::CGUITable(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DFont *_font, bool _fontowner): CGUIControl(_topLeft, _size)
{
	m_font = _font;
	m_fontowner = _fontowner;
	m_fontcolor = 0xff000000;
	m_cells = 0;
	m_colwidths = 0;
	m_colaligns = 0;
}


CGUITable::~CGUITable()
{
	if (m_font && m_fontowner)
		delete m_font;
	if (m_cells)
		delete [] m_cells;
	if (m_colwidths)
		delete [] m_colwidths;
	if (m_colaligns)
		delete [] m_colaligns;
}

void CGUITable::Render()
{
	if (!m_visible)
		return;


	CIwMat2D tm;
	tm.SetIdentity();
	tm.SetTrans(m_topLeft);
	Iw2DSetTransformMatrix(tm);

	//Iw2DSetColour(0x99999999);
	//Iw2DFillRect(CIwSVec2(0, 0), m_size);
	Iw2DSetColour(0xff000000);
	int16 rh = m_size.y / m_rowcount;
	int16 cw = 0;
	int16 l = 0;
	int16 t = 0;
	for(int row = 0; row < m_rowcount; row++)
	{
		l = 0;
		if (row % 2)
			Iw2DSetColour(0xffcccccc);
		else
			Iw2DSetColour(0xffeeeeee);
		Iw2DFillRect(CIwSVec2(0, t), CIwSVec2(m_size.x, rh));

		Iw2DSetColour(0xff000000);
		for(int col = 0; col < m_colcount; col++)
		{
			cw = m_size.x * m_colwidths[col] / 100;

			Iw2DDrawString(GetCellText(col, row), CIwSVec2(l + 2, t), CIwSVec2(cw - 4, rh), m_colaligns[col], IW_2D_FONT_ALIGN_CENTRE);
			
			l += cw;
		}
		t += rh;
	}
	//
	Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
}

void CGUITable::SetTableSize(int _colcount, int _rowcount)
{
	if (m_cells)
		delete [] m_cells;
	m_colcount = _colcount;
	m_rowcount = _rowcount;
	m_cells = new char[m_colcount * m_rowcount][128];
	memset(m_cells, 0, m_colcount * m_rowcount * 128);
	m_colwidths = new int16[m_colcount];
	m_colaligns = new CIw2DFontAlign[m_colcount];
	for (int i = 0; i < m_colcount; i++)
	{
		m_colwidths[i] = 100 / m_colcount;
		m_colaligns[i] = IW_2D_FONT_ALIGN_LEFT;
	}
}


CGUIListView::CGUIListView(CIwSVec2 _topLeft, CIwSVec2 _size, CIw2DFont *_font, bool _fontowner): CGUIControl(_topLeft, _size)
{
	m_font = _font;
	m_fontowner = _fontowner;
	m_fontcolor = 0xff000000;
	m_itemsize = _size.y / 5;
	m_itemsubtextsize = 0;
	m_down = false;
	m_over = false;
	m_offsetx = 0;
	m_offsety = 0;
	m_itemindex = -1;
	m_needselect = true;
	m_needimages = true;
	m_scrollspeed = 0;
	m_direction = dVertical;
}

CGUIListView::~CGUIListView()
{
	if (m_fontowner)
		delete m_font;
	Clear();
}

void CGUIListView::Render()
{
	if (!m_visible)
		return;

	CIwMat2D tm;
	tm.SetIdentity();
	tm.SetTrans(m_topLeft);
	Iw2DSetTransformMatrix(tm);
	IwGxSetScissorScreenSpace(m_topLeft.x, m_topLeft.y, m_size.x, m_size.y);


	switch(m_direction)
	{
		case dVertical:	RenderVertical(); break;
		case dHorizontal: RenderHorizontal(); break;
	}
	
	IwGxClearScissorScreenSpace();
	Iw2DSetTransformMatrix(CIwMat2D::g_Identity);
}

void CGUIListView::RenderVertical()
{
	Iw2DSetColour(0xffeeeeee);
	Iw2DFillRect(CIwSVec2(0, 0), m_size);
	int16 t = m_offsety;
	for (uint32 i = 0; i < m_items.size() && t < m_size.y; i++)
	{

		if (m_items[i]->visible)
		{
			if (t + m_itemsize > 0)
			{
				int16 l = m_offsetx;
				uint32 clr = 0xffeeeeee;
				if (i % 2)
					clr -= 0x00111111;
				if ((int32)i == m_itemindex)
					clr -= 0x00222222;	
				Iw2DSetColour(clr);
				Iw2DFillRect(CIwSVec2(l, t), CIwSVec2(m_size.x, m_itemsize));
				Iw2DSetColour(0xffffffff);
				Iw2DDrawLine(CIwSVec2(l, t), CIwSVec2(l + m_size.x, t));
				Iw2DSetColour(0xffbbbbbb);
				Iw2DDrawLine(CIwSVec2(l, t + m_itemsize - 1), CIwSVec2(l + m_size.x, t + m_itemsize - 1));
				if (m_needimages && m_items[i]->image)
				{
					Iw2DSetColour(0xffffffff);
					int16 iml, imt, imw, imh;
					imw = m_items[i]->image->GetWidth();
					imh = m_items[i]->image->GetHeight();
					if (imw > imh)
					{
						imh = imh * m_itemsize / imw;
						imw = m_itemsize;
						iml = l;
						imt = t + (m_itemsize - imh) / 2;
					}
					else
					{
						imw = imw * m_itemsize / imh;
						imh = m_itemsize;
						iml = l + (m_itemsize - imw) / 2;
						imt = t;
					}
					Iw2DDrawImage(m_items[i]->image, CIwSVec2(iml, imt), CIwSVec2(imw, imh));
					l += m_itemsize;
				}
				Iw2DSetColour(m_items[i]->fontcolor);
				Iw2DDrawString(m_items[i]->captionleft, CIwSVec2(l + 4 , t), CIwSVec2(m_size.x - l - 8, m_itemsize - m_itemsubtextsize), IW_2D_FONT_ALIGN_LEFT, IW_2D_FONT_ALIGN_CENTRE);
				Iw2DSetColour(m_items[i]->fontcolor);
				Iw2DDrawString(m_items[i]->captionright, CIwSVec2(l + 4 , t), CIwSVec2(m_size.x - l - 8, m_itemsize - m_itemsubtextsize), IW_2D_FONT_ALIGN_RIGHT, IW_2D_FONT_ALIGN_CENTRE);
				if (m_itemsubtextsize > 0)
				{
					Iw2DSetColour(m_items[i]->fontcolor);
					Iw2DDrawString(m_items[i]->subtext, CIwSVec2(l + 4 , t + m_itemsize - m_itemsubtextsize), CIwSVec2(m_size.x - l - 8, m_itemsubtextsize), IW_2D_FONT_ALIGN_RIGHT, IW_2D_FONT_ALIGN_CENTRE);
				}
			}
			t += m_itemsize;
		}
	}
}

void CGUIListView::RenderHorizontal()
{
	int16 l = m_offsetx;
	for (uint32 i = 0; i < m_items.size() && l < m_size.x; i++)
	{

		if (m_items[i]->visible)
		{
			if (l + m_itemsize > 0)
			{
				int16 t = m_offsety;
				uint32 clr = 0xffffffff;
				//if (i % 2)
				//	clr -= 0x00111111;
				if ((int32)i == m_itemindex)
					clr -= 0x00222222;	
				Iw2DSetColour(clr);
				//Iw2DFillRect(CIwSVec2(l, t), CIwSVec2(m_size.x, m_itemsize));
				//Iw2DSetColour(0xffffffff);
				//Iw2DDrawLine(CIwSVec2(l, t), CIwSVec2(l + m_size.x, t));
				//Iw2DSetColour(0xffbbbbbb);
				//Iw2DDrawLine(CIwSVec2(l, t + m_itemsize - 1), CIwSVec2(l + m_size.x, t + m_itemsize - 1));
				int imagesize;
				if (m_needimages && m_items[i]->image)
				{
					int16 iml, imt, imw, imh;
					imw = m_items[i]->image->GetWidth();
					imh = m_items[i]->image->GetHeight();
					imagesize = m_itemsize * 3 / 4;
					if (imw > imh)
					{
						imh = imh * imagesize / imw;
						imw = imagesize;
						iml = l + (m_itemsize - imw) / 2;
						imt = t;
					}
					else
					{
						imw = imw * imagesize / imh;
						imh = imagesize;
						iml = l + (m_itemsize - imw) / 2;
						imt = t;
					}
					Iw2DDrawImage(m_items[i]->image, CIwSVec2(iml, imt), CIwSVec2(imw, imh));
				}
				else
				{
					imagesize = 0;
				}
				int tt = t + imagesize;
				Iw2DSetColour(m_items[i]->fontcolor);
				Iw2DDrawString(m_items[i]->captionleft, CIwSVec2(l , tt + 4), CIwSVec2(m_itemsize, m_size.y - tt - 4), IW_2D_FONT_ALIGN_CENTRE, IW_2D_FONT_ALIGN_TOP);
				Iw2DSetColour(m_items[i]->fontcolor);
				Iw2DDrawString(m_items[i]->captionright, CIwSVec2(l, tt + 4), CIwSVec2(m_itemsize, m_size.y - tt - 4), IW_2D_FONT_ALIGN_CENTRE, IW_2D_FONT_ALIGN_BOTTOM);

			}
			l += m_itemsize;
		}
	}
}

void CGUIListView::Update(int deltaTimeMs)
{
	switch(m_direction)
	{
		case dVertical:	UpdateVertical(deltaTimeMs); break;
		case dHorizontal: UpdateHorizontal(deltaTimeMs); break;
	}
}

void CGUIListView::UpdateVertical(int deltaTimeMs)
{
	if (!m_down)
	{

		if (m_offsety > 0)
		{
			if (m_offsety > 20) 
				m_offsety -= deltaTimeMs * m_offsety / 100;
			else
				m_offsety -= deltaTimeMs / 5;
			if (m_offsety < 0)
				m_offsety = 0;
			m_scrollspeed = 0;
		}
		else if (m_offsety < m_offsetymin)
		{
			if (m_offsety < m_offsetymin - 20) 
				m_offsety += deltaTimeMs * (m_offsetymin - m_offsety) / 100;
			else
				m_offsety += deltaTimeMs / 5;
			if (m_offsety > m_offsetymin)
				m_offsety = m_offsetymin;
			m_scrollspeed = 0;
		} 
		else if (m_scrollspeed)
		{
			m_offsety += m_scrollspeed * deltaTimeMs / 1000;
			if (m_scrollspeed > 0)
			{
				m_scrollspeed -= deltaTimeMs;
				if (m_scrollspeed < 0)
					m_scrollspeed = 0;
			}
			else if (m_scrollspeed < 0)
			{
				m_scrollspeed += deltaTimeMs;
				if (m_scrollspeed > 0)
					m_scrollspeed = 0;
			}
			if (m_offsety > 0 || m_offsety < m_offsetymin)
				m_scrollspeed = 0;

		}
	}
	else
	{
		m_scrollspeed = (m_offsety - m_offsetprev) * 1000 / deltaTimeMs;
		m_offsetprev = m_offsety;
	}
}

void CGUIListView::UpdateHorizontal(int deltaTimeMs)
{
	if (!m_down)
	{

		if (m_offsetx > 0)
		{
			if (m_offsetx > 20) 
				m_offsetx -= deltaTimeMs * m_offsetx / 100;
			else
				m_offsetx -= deltaTimeMs / 5;
			if (m_offsetx < 0)
				m_offsetx = 0;
			m_scrollspeed = 0;
		}
		else if (m_offsetx < m_offsetxmin)
		{
			if (m_offsetx < m_offsetxmin - 20) 
				m_offsetx += deltaTimeMs * (m_offsetxmin - m_offsetx) / 100;
			else
				m_offsetx += deltaTimeMs / 5;
			if (m_offsetx > m_offsetxmin)
				m_offsetx = m_offsetxmin;
			m_scrollspeed = 0;
		} 
		else if (m_scrollspeed)
		{
			m_offsetx += m_scrollspeed * deltaTimeMs / 1000;
			if (m_scrollspeed > 0)
			{
				m_scrollspeed -= deltaTimeMs;
				if (m_scrollspeed < 0)
					m_scrollspeed = 0;
			}
			else if (m_scrollspeed < 0)
			{
				m_scrollspeed += deltaTimeMs;
				if (m_scrollspeed > 0)
					m_scrollspeed = 0;
			}
			if (m_offsetx > 0 || m_offsetx < m_offsetxmin)
				m_scrollspeed = 0;

		}
	}
	else
	{
		m_scrollspeed = (m_offsetx - m_offsetprev) * 1000 / deltaTimeMs;
		m_offsetprev = m_offsetx;
	}
}

void CGUIListView::UpdateVisibleItemsCount()
{
	m_visibleitemscount = 0; 
	for(uint32 i = 0; i < m_items.size(); i++) 
		m_visibleitemscount += (int16)m_items[i]->visible;
	switch(m_direction)
	{
		case dVertical:	
		{
			m_offsetymin = m_size.y - GetAllItemsSize();
			if (m_offsetymin > 0)
				m_offsetymin = 0;
		}
		break;
		case dHorizontal:	
		{
			m_offsetxmin = m_size.x - GetAllItemsSize();
			if (m_offsetxmin > 0)
				m_offsetxmin = 0;
		}
		break;
	}
	



};

void CGUIListView::MoveItemToCenter(int index)
{
	switch(m_direction)
	{
		case dVertical:	
		{
			m_offsety = (m_size.y - m_itemsize) / 2 - index * m_itemsize;
			if (m_offsety > 0)
				m_offsety = 0;
			else if (m_offsety < m_offsetymin)
				m_offsety = m_offsetymin;
		}
		break;
		case dHorizontal:	
		{
			m_offsetx = (m_size.x - m_itemsize) / 2 - index * m_itemsize;
			if (m_offsetx > 0)
				m_offsetx = 0;
			else if (m_offsetx < m_offsetxmin)
				m_offsetx = m_offsetxmin;
		}
		break;
	}

}


int CGUIListView::AddItem(const char *resource, const char *caption)
{
	int index = m_items.push_back(new CGUIListItem);
	CGUIListItem *item =  m_items[index];
	if (resource != NULL)
		item->image = Iw2DCreateImageResource(resource);
	else
		item->image = 0;
	item->data = NULL;
	item->visible = true;
	item->fontcolor = m_fontcolor;
	if (caption)
		sprintf(item->captionleft, caption);
	else
		item->captionleft[0] = '\0';
	//sprintf(item->captionright, "");
	item->captionright[0] = '\0';
	item->subtext[0] = '\0';
	//sprintf(item->subtext, "");
	UpdateVisibleItemsCount();
	return index;
}

void CGUIListView::Clear()
{
	m_offsetx = 0;
	m_offsety = 0;
	CIwArray<CGUIListItem *>::iterator it;
	for (it = m_items.begin(); it != m_items.end(); it++)
	{
		if ((*it)->image)
			delete (*it)->image; 
		delete (*it);
	}
	m_items.clear();
	UpdateVisibleItemsCount();
}

bool CGUIListView::Down(int32 x, int32 y)
{
	bool res = (m_enabled &&
		        m_visible &&
		        x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	m_down = res;
	m_over = res;
	if (res)
	{
		int32 index = -1;
		switch(m_direction)
		{
			case dVertical:	
			{
				int16 t = get_topLeft().y + m_offsety;
				for (uint32 i = 0; i < m_items.size() && index == -1 ; i++)
				{
					if (m_items[i]->visible)
					{
						if (y >= t && y < t + m_itemsize)
							index = i;
						t += m_itemsize;
					}
				}
			}
			break;
			case dHorizontal:	
			{
				int16 l = get_topLeft().x + m_offsetx;
				for (uint32 i = 0; i < m_items.size() && index == -1 ; i++)
				{
					if (m_items[i]->visible)
					{
						if (x >= l && x < l + m_itemsize)
							index = i;
						l += m_itemsize;
					}
				}
			}
			break;
		}
		
		if (index >= 0 && index < (int32)m_items.size() && m_needselect)
			set_itemindex(index);
		m_downx = x;
		m_downy = y;
		m_offsetx0 = m_offsetx;
		m_offsety0 = m_offsety;
		m_offsetprev = m_offsety0;
	}
	return res;
}

bool CGUIListView::Up(int32 x, int32 y)
{
	bool res = (m_enabled &&
				m_visible &&
				x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	m_down = false;
	m_over = false;
	return res;
}

bool CGUIListView::Move(int32 x, int32 y)
{
	m_over =   (m_enabled &&
				m_visible &&
				x > get_topLeft().x &&
				x < get_topLeft().x + get_size().x &&
				y > get_topLeft().y &&
				y < get_topLeft().y + get_size().y);
	if (m_down)
	{
		switch(m_direction)
		{
			case dVertical:	
			{
				m_offsety = m_offsety0 + y - m_downy;
			}
			break;
			case dHorizontal:	
			{
				m_offsetx = m_offsetx0 + x - m_downx;
			}
			break;
		}
		
	}
	return m_over;
}