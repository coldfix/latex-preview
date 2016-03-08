/////////////////////////////////////////////////////////////////////////////
// Name:        wximagectrl.cpp
// Purpose:     
// Author:      Thomas Gl‰ﬂle
// Modified by: 
// Created:     Thu 26 Nov 2009 18:25:10 CET
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wximagectrl.h"

////@begin XPM images
////@end XPM images


/*
 * wxImageCtrl type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxImageCtrl, wxControl )


/*
 * wxImageCtrl event table definition
 */

BEGIN_EVENT_TABLE( wxImageCtrl, wxControl )

////@begin wxImageCtrl event table entries
    EVT_SIZE( wxImageCtrl::OnSize )
    EVT_PAINT( wxImageCtrl::OnPaint )

////@end wxImageCtrl event table entries

END_EVENT_TABLE()


/*
 * wxImageCtrl constructors
 */

wxImageCtrl::wxImageCtrl()
{
    Init();
}

wxImageCtrl::wxImageCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
    Init();
    Create(parent, id, pos, size, style);
}


/*
 * wxImageCtrl creator
 */

bool wxImageCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
////@begin wxImageCtrl creation
    wxControl::Create(parent, id, pos, size, style);
    CreateControls();
////@end wxImageCtrl creation
    return true;
}


/*
 * wxImageCtrl destructor
 */

wxImageCtrl::~wxImageCtrl()
{
////@begin wxImageCtrl destruction
////@end wxImageCtrl destruction
}


/*
 * Member initialisation
 */

void wxImageCtrl::Init()
{
////@begin wxImageCtrl member initialisation
////@end wxImageCtrl member initialisation
}


/*
 * Control creation for wxImageCtrl
 */

void wxImageCtrl::CreateControls()
{    
////@begin wxImageCtrl content construction
////@end wxImageCtrl content construction
}


/*
 * Should we show tooltips?
 */

bool wxImageCtrl::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap wxImageCtrl::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin wxImageCtrl bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end wxImageCtrl bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon wxImageCtrl::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin wxImageCtrl icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end wxImageCtrl icon retrieval
}


void wxImageCtrl::SetImage(const wxBitmap& bmp)
{
	m_img = bmp;
	wxSize size = DoGetBestSize();
	SetInitialSize( size );
	SetMinSize( size );
	SetSize( size );

}

wxSize wxImageCtrl::DoGetBestSize() const
{
	if (!m_img.IsOk())
		return wxSize(40, 20);
	return wxSize(
			m_img.GetWidth() + 40,
			m_img.GetHeight() + 20);
}



/*
 * wxEVT_SIZE event handler for ID_FOREIGN_IMAGE
 */

void wxImageCtrl::OnSize( wxSizeEvent& event )
{
	Refresh();
    event.Skip();
}


/*
 * wxEVT_PAINT event handler for ID_FOREIGN_IMAGE
 */

void wxImageCtrl::OnPaint( wxPaintEvent& event )
{
	wxPaintDC dc(this);

	// dc.SetBackground(*wxWHITE_BRUSH);
	// dc.Clear();
	if (!m_img.IsOk()) {
		return;
	}

	dc.DrawBitmap( m_img,
			(dc.GetSize().x - m_img.GetWidth() + 1) / 2,
			(dc.GetSize().y - m_img.GetHeight() + 1) / 2,
			true );
}

