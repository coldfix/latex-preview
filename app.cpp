/////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     Fri 20 Nov 2009 02:00:32 CET
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

#include "app.h"

////@begin XPM images
////@end XPM images


/*
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( LatexPreviewApp )
////@end implement app


/*
 * LatexPreviewApp type definition
 */

IMPLEMENT_CLASS( LatexPreviewApp, wxApp )


/*
 * LatexPreviewApp event table definition
 */

BEGIN_EVENT_TABLE( LatexPreviewApp, wxApp )

////@begin LatexPreviewApp event table entries
////@end LatexPreviewApp event table entries

END_EVENT_TABLE()


/*
 * Constructor for LatexPreviewApp
 */

LatexPreviewApp::LatexPreviewApp()
{
    Init();
}


/*
 * Member initialisation
 */

void LatexPreviewApp::Init()
{
////@begin LatexPreviewApp member initialisation
////@end LatexPreviewApp member initialisation
}

/*
 * Initialisation for LatexPreviewApp
 */

bool LatexPreviewApp::OnInit()
{
////@begin LatexPreviewApp initialisation
    // Remove the comment markers above and below this block
    // to make permanent changes to the code.

#if wxUSE_XPM
    wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
    wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
    wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
    wxImage::AddHandler(new wxGIFHandler);
#endif
    LatexPreviewWindow* mainWindow = new LatexPreviewWindow( NULL );
    mainWindow->Show(true);
////@end LatexPreviewApp initialisation

    return true;
}


/*
 * Cleanup for LatexPreviewApp
 */

int LatexPreviewApp::OnExit()
{
////@begin LatexPreviewApp cleanup
    return wxApp::OnExit();
////@end LatexPreviewApp cleanup
}

