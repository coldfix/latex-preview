/////////////////////////////////////////////////////////////////////////////
// Name:        aboutbox.cpp
// Purpose:     
// Author:      Thomas Gläßle
// Modified by: 
// Created:     Fri 25 Feb 2011 01:54:46 AM CET
// RCS-ID:      
// Copyright:   Thomas Gläßle (C) 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Fri 25 Feb 2011 01:54:46 AM CET

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "aboutbox.h"
#endif

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

#include "aboutbox.h"

////@begin XPM images
////@end XPM images


namespace wxtk
{

/*!
 * AboutBox type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AboutBox, wxDialog )


/*!
 * AboutBox event table definition
 */

BEGIN_EVENT_TABLE( AboutBox, wxDialog )

////@begin AboutBox event table entries
    EVT_TEXT_URL( wxID_TEXTCTRL_DESCRIPTION, AboutBox::OnTextctrlDescriptionClickUrl )

////@end AboutBox event table entries

END_EVENT_TABLE()


/*!
 * AboutBox constructors
 */

AboutBox::AboutBox()
{
    Init();
}

AboutBox::AboutBox( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * AboutBox creator
 */

bool AboutBox::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AboutBox creation
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AboutBox creation
    return true;
}


/*!
 * AboutBox destructor
 */

AboutBox::~AboutBox()
{
////@begin AboutBox destruction
////@end AboutBox destruction
}


/*!
 * Member initialisation
 */

void AboutBox::Init()
{
////@begin AboutBox member initialisation
    m_copyright = wxT("(C) Thomas Gläßle");
    m_ctrl_bitmap = NULL;
    m_ctrl_description = NULL;
////@end AboutBox member initialisation
}


/*!
 * Control creation for AboutBox
 */

void AboutBox::CreateControls()
{    
////@begin AboutBox content construction
    // Generated by DialogBlocks, Fri 25 Feb 2011 02:00:53 AM CET (unregistered)

    AboutBox* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    m_ctrl_bitmap = new wxStaticBitmap( itemDialog1, wxID_STATICBITMAP_BIGICON, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(m_ctrl_bitmap, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC_APPNAME, _("Static text"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    itemBoxSizer4->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC_COPYRIGHT, _("Static text"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    m_ctrl_description = new wxTextCtrl( itemDialog1, wxID_TEXTCTRL_DESCRIPTION, wxEmptyString, wxDefaultPosition, wxSize(400, 200), wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
    itemBoxSizer3->Add(m_ctrl_description, 1, wxGROW|wxALL, 5);

    wxStaticLine* itemStaticLine10 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer3->Add(itemStaticLine10, 0, wxGROW|wxALL, 5);

    wxButton* itemButton11 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    // Set validators
    itemStaticText6->SetValidator( wxGenericValidator(& m_appname) );
    itemStaticText8->SetValidator( wxGenericValidator(& m_copyright) );
    m_ctrl_description->SetValidator( wxGenericValidator(& m_description) );
////@end AboutBox content construction
}


/*
	others
*/

bool AboutBox::TransferDataToWindow()
{
	if (wxDialog::TransferDataToWindow()) {
        m_ctrl_bitmap->SetBitmap(m_bigicon);
		GetSizer()->Fit(this);
		GetSizer()->SetSizeHints(this);
		return true;
    }
	return false;
}





/*!
 * wxEVT_COMMAND_TEXT_URL event handler for wxID_TEXTCTRL_DESCRIPTION
 */

void AboutBox::OnTextctrlDescriptionClickUrl( wxTextUrlEvent& event )
{
	const wxMouseEvent& me = event.GetMouseEvent();
	if (me.LeftDown()) {
		wxString url = m_ctrl_description->GetRange(event.GetURLStart(), event.GetURLEnd());
		::wxLaunchDefaultBrowser(url);
	}
    else
		event.Skip();
}


/*!
 * Should we show tooltips?
 */

bool AboutBox::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AboutBox::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AboutBox bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AboutBox bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AboutBox::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AboutBox icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AboutBox icon retrieval
}


} // namespace wxtk

