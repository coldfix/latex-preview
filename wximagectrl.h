/////////////////////////////////////////////////////////////////////////////
// Name:        wximagectrl.h
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     Thu 26 Nov 2009 18:25:10 CET
// RCS-ID:
// Copyright:
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXIMAGECTRL_H_
#define _WXIMAGECTRL_H_


/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxImageCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_WXIMAGECTRL_STYLE wxRAISED_BORDER
#define SYMBOL_WXIMAGECTRL_IDNAME ID_FOREIGN_IMAGE
#define SYMBOL_WXIMAGECTRL_SIZE wxSize(180, 50)
#define SYMBOL_WXIMAGECTRL_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * wxImageCtrl class declaration
 */

class wxImageCtrl: public wxControl
{
    DECLARE_DYNAMIC_CLASS( wxImageCtrl )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    wxImageCtrl();
    wxImageCtrl(wxWindow* parent, wxWindowID id = ID_FOREIGN_IMAGE, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(180, 50), long style = wxRAISED_BORDER);

    /// Creation
    bool Create(wxWindow* parent, wxWindowID id = ID_FOREIGN_IMAGE, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(180, 50), long style = wxRAISED_BORDER);

    /// Destructor
    ~wxImageCtrl();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

    void SetImage(const wxBitmap& bmp);
    wxSize DoGetBestSize() const;


////@begin wxImageCtrl event handler declarations

    /// wxEVT_SIZE event handler for ID_FOREIGN_IMAGE
    void OnSize( wxSizeEvent& event );

    /// wxEVT_PAINT event handler for ID_FOREIGN_IMAGE
    void OnPaint( wxPaintEvent& event );

////@end wxImageCtrl event handler declarations

////@begin wxImageCtrl member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end wxImageCtrl member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin wxImageCtrl member variables
    /// Control identifiers
    enum {
        ID_FOREIGN_IMAGE = 10002
    };
////@end wxImageCtrl member variables

    wxBitmap m_img;
};

#endif
    // _WXIMAGECTRL_H_
