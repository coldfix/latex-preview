/////////////////////////////////////////////////////////////////////////////
// Name:        window.h
// Purpose:     
// Author:      Thomas Gl‰ﬂle
// Modified by: 
// Created:     Fri 20 Nov 2009 02:01:08 CET
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _LATEXPREVIEWWINDOW_H_
#define _LATEXPREVIEWWINDOW_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/notebook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxNotebook;
class wxWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define SYMBOL_LATEXPREVIEWWINDOW_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_LATEXPREVIEWWINDOW_TITLE _("LatexPreview")
#define SYMBOL_LATEXPREVIEWWINDOW_IDNAME ID_LATEXPREVIEWWINDOW
#define SYMBOL_LATEXPREVIEWWINDOW_SIZE wxSize(400, 300)
#define SYMBOL_LATEXPREVIEWWINDOW_POSITION wxDefaultPosition
////@end control identifiers



namespace filetype {
	enum {
		png,
		gif
	};
};

struct execution_info
{
	int exitcode;
	wxString cmd,
			 out,
			 err;
};


struct ConfigData
{
	int width, height,
		select_start, select_end;
//	int select_col_start, select_col_end;
	wxString textro, texplate;
	bool autorender, transparent;
	int file_type;
};


/*!
 * LatexPreviewWindow class declaration
 */

class LatexPreviewWindow: public wxFrame
{    
    DECLARE_DYNAMIC_CLASS( LatexPreviewWindow )
    DECLARE_EVENT_TABLE()


public:
    /// Constructors
    LatexPreviewWindow();
    LatexPreviewWindow( wxWindow* parent, wxWindowID id = SYMBOL_LATEXPREVIEWWINDOW_IDNAME, const wxString& caption = SYMBOL_LATEXPREVIEWWINDOW_TITLE, const wxPoint& pos = SYMBOL_LATEXPREVIEWWINDOW_POSITION, const wxSize& size = SYMBOL_LATEXPREVIEWWINDOW_SIZE, long style = SYMBOL_LATEXPREVIEWWINDOW_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_LATEXPREVIEWWINDOW_IDNAME, const wxString& caption = SYMBOL_LATEXPREVIEWWINDOW_TITLE, const wxPoint& pos = SYMBOL_LATEXPREVIEWWINDOW_POSITION, const wxSize& size = SYMBOL_LATEXPREVIEWWINDOW_SIZE, long style = SYMBOL_LATEXPREVIEWWINDOW_STYLE );

    /// Destructor
    ~LatexPreviewWindow();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();


	void GetDefaultConfig(ConfigData&);
	wxString GetDefaultTemplate();
	wxString GetDefaultFormula();
	bool LoadConfig(ConfigData&);
	bool SaveConfig(const ConfigData&);


	bool BuildTex(
			const wxString& text,
			const wxString& texplate,
			const wxString& file_tex,
			execution_info& info );

	bool BuildDvi(
			const wxString& file_tex,
			const wxString& file_dvi,
			execution_info& info );

	bool BuildImg(
			const wxString& file_dvi,
			const wxString& file_img,
			int ft,
			bool transparent,
			execution_info& info );


	bool Rebuild(bool switch_to_log = false);

	bool Build(
		const wxString& text,
		const wxString& texplate,
		wxBitmap& img,
		execution_info& cmd );

	void SetImage(const wxBitmap& img);

	void SetDirty();			// require rebuild
	bool ShouldBuild();




////@begin LatexPreviewWindow event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_LATEXPREVIEWWINDOW
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE
    void OnSaveClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_REVERT_TO_SAVED
    void OnRevertToSavedClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_DEFAULT
    void OnDefaultClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
    void OnOpenClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE_TPL
    void OnSaveTplClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_REVERT_TO_SAVED_TPL
    void OnRevertToSavedTplClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_DEFAULT_TPL
    void OnDefaultTplClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE_TEX
    void OnSaveTexClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVEAS
    void OnSaveasClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_FT_PNG
    void OnMenuitemFtPngClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MENUITEM_FT_PNG
    void OnMenuitemFtPngUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_FT_GIF
    void OnMenuitemFtGifClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MENUITEM_FT_GIF
    void OnMenuitemFtGifUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_BG_TRANSPARENT
    void OnMenuitemBgTransparentClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MENUITEM_BG_TRANSPARENT
    void OnMenuitemBgTransparentUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_AUTORENDER
    void OnMenuitemAutorenderClick( wxCommandEvent& event );

    /// wxEVT_UPDATE_UI event handler for ID_MENUITEM_AUTORENDER
    void OnMenuitemAutorenderUpdate( wxUpdateUIEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_EXIT
    void OnExitClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
    void OnAboutClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK
    void OnNotebookPageChanged( wxNotebookEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_INPUT
    void OnTextctrlInputTextUpdated( wxCommandEvent& event );

    /// wxEVT_SIZE event handler for ID_FOREIGN_IMAGE_PREVIEW
    void OnSize( wxSizeEvent& event );

    /// wxEVT_PAINT event handler for ID_FOREIGN_IMAGE_PREVIEW
    void OnPaint( wxPaintEvent& event );

    /// wxEVT_LEFT_DOWN event handler for ID_FOREIGN_IMAGE_PREVIEW
    void OnLeftDown( wxMouseEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REFRESH
    void OnRefreshClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
    void OnCloseClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_TEMPLATE
    void OnTextctrlTemplateTextUpdated( wxCommandEvent& event );

////@end LatexPreviewWindow event handler declarations

////@begin LatexPreviewWindow member function declarations

    bool GetAutorender() const { return m_autorender ; }
    void SetAutorender(bool value) { m_autorender = value ; }

    int GetFiletype() const { return m_filetype ; }
    void SetFiletype(int value) { m_filetype = value ; }

    wxString GetTemplate() const { return m_template ; }
    void SetTemplate(wxString value) { m_template = value ; }

    bool GetTransparent() const { return m_transparent ; }
    void SetTransparent(bool value) { m_transparent = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end LatexPreviewWindow member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LatexPreviewWindow member variables
    wxNotebook* m_notebook;
    wxPanel* m_panel_formula;
    wxTextCtrl* m_control_input;
    wxPanel* m_panel_image;
    wxWindow* m_control_image;
    wxTextCtrl* m_control_template;
    wxPanel* m_panel_log;
    wxTextCtrl* m_control_log_operation;
    wxTextCtrl* m_control_log_exitcode;
    wxTextCtrl* m_control_log;
    bool m_autorender;
    int m_filetype;
    wxString m_template;
    bool m_transparent;
    /// Control identifiers
    enum {
        ID_LATEXPREVIEWWINDOW = 10000,
        ID_MENU_FORMULA = 10018,
        ID_MENU_TEMPLATE = 10017,
        wxID_SAVE_TPL = 10004,
        wxID_REVERT_TO_SAVED_TPL = 10019,
        wxID_DEFAULT_TPL = 10020,
        wxID_SAVE_TEX = 10023,
        ID_MENU_IMAGE = 10009,
        ID_MENUITEM_FT_PNG = 10006,
        ID_MENUITEM_FT_GIF = 10007,
        ID_MENUITEM_BG_TRANSPARENT = 10008,
        ID_MENUITEM_AUTORENDER = 10003,
        ID_PANEL = 10021,
        ID_NOTEBOOK = 10005,
        ID_PANEL_PREVIEW = 10010,
        ID_TEXTCTRL_INPUT = 10001,
        ID_PANEL1 = 10022,
        ID_FOREIGN_IMAGE_PREVIEW = 10002,
        ID_PANEL_TEMPLATE = 10012,
        ID_TEXTCTRL_TEMPLATE = 10014,
        ID_PANEL_LOG = 10011,
        ID_TEXTCTRL_LOG_OPERATION = 10015,
        ID_TEXTCTRL_LOG_EXITCODE = 10016,
        ID_TEXTCTRL_LOG = 10013
    };
////@end LatexPreviewWindow member variables

	bool m_make_filename;
	wxString m_filename, m_filename_img;
	wxString m_config;
	bool m_dirty, m_resize_frame;
	wxBitmap m_img;
};

#endif

