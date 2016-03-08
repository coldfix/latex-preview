/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     Fri 20 Nov 2009 02:00:32 CET
// RCS-ID:
// Copyright:
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _LATEXPREVIEWAPP_H_
#define _LATEXPREVIEWAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "window.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * LatexPreviewApp class declaration
 */

class LatexPreviewApp: public wxApp
{
    DECLARE_CLASS( LatexPreviewApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    LatexPreviewApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin LatexPreviewApp event handler declarations

////@end LatexPreviewApp event handler declarations

////@begin LatexPreviewApp member function declarations

////@end LatexPreviewApp member function declarations

////@begin LatexPreviewApp member variables
////@end LatexPreviewApp member variables
};

/*!
 * Application instance declaration
 */

////@begin declare app
DECLARE_APP(LatexPreviewApp)
////@end declare app

#endif
    // _LATEXPREVIEWAPP_H_
