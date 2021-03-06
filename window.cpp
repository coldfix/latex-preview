/////////////////////////////////////////////////////////////////////////////
// Name:        window.cpp
// Purpose:
// Author:      Thomas Gläßle
// Modified by:
// Created:     Fri 20 Nov 2009 02:01:08 CET
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
#include "wx/imaglist.h"
#include "wximagectrl.h"
////@end includes

#include "window.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
////@begin XPM images
#include "icon1.xpm"
////@end XPM images
#pragma GCC diagnostic pop


#include <wx/wfstream.h>        // wxFileOutputStream
#include <wx/txtstrm.h>         // wxTextOutputStream
#include <wx/bitmap.h>          // wxBitmap
#include <wx/dcbuffer.h>        // wxAutoBufferedPaintDC
#include <wx/filename.h>        // wxFileName
#include <wx/stdpaths.h>        // wxStandardPaths
#include <wx/filedlg.h>         // wxFileDialog
#include <wx/dataobj.h>         // wxFileDataObject
#include <wx/dnd.h>             // wxDropSource
#include <wx/fileconf.h>        // wxFileConfig


#include <string>
#include <vector>
#include <boost/process.hpp>

#include <iosfwd>                           // streamsize
#include <boost/iostreams/categories.hpp>   // sink_tag
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>

#include "aboutbox.h"


namespace bp =  boost::process;
namespace bio = boost::iostreams;


/*
 * TODO: put this stuff in proper headers
 */

    /* to do: put into header */
    inline wxString to_wx(const std::string& stdstr)
    {
        return wxString(stdstr.c_str(), wxConvUTF8);
    }

    inline std::string to_std(const wxString& wxstr)
    {
        return std::string(wxstr.mb_str(wxConvUTF8));
    }

class string_sink
{
    std::string str;
public:
    typedef char      char_type;
    typedef bio::sink_tag  category;

    std::streamsize write(const char* s, std::streamsize n)
    {
        str.append(s, n);
        return n;
    }

    std::string get() const
    {
        return str;
    }
};




bool execute(
        const std::vector<std::string>& args,
        execution_info& info)
{
    bp::pipe out_pipe, err_pipe;
    {
        bp::child c(
            args,
            bp::std_out > out_pipe,
            bp::std_err > err_pipe,
            bp::std_in < bp::null);
        c.wait();
        info.exitcode = c.exit_code();
    }

    bio::file_descriptor_source
        out_source(out_pipe.native_source(), bio::close_handle),
        err_source(err_pipe.native_source(), bio::close_handle);
    string_sink out, err;
    bio::copy(out_source, out);
    bio::copy(err_source, err);

    info.cmd = to_wx(args[0]);
    info.out = to_wx(out.get());
    info.err = to_wx(err.get());

    return info.exitcode == 0;
}


wxString insert_marker = wxT("<!-- ... -->");



    class ChangeCwd
    {
        wxString backup_cwd_;
    public:
        ChangeCwd(const wxString& cwd)
        {
            backup_cwd_ = wxFileName::GetCwd();
            wxFileName::SetCwd(cwd);
        }
        ~ChangeCwd()
        {
            wxFileName::SetCwd(backup_cwd_);
        }
    };

    wxString ReadStream(wxInputStream& stream)
    {
        wxTextInputStream txt(stream);
        wxString str, line;
        while (!stream.Eof())
            str += txt.ReadLine() + wxT('\n');
        return str;
    }

    std::string ReadStream(std::istream& stream)
    {
        std::string str, line;
        while (std::getline(stream, line))
            str += line + '\n';
        return str;
    }

    bool LoadFile(const wxString& filename, wxString& contents)
    {
        if (!wxFileName::IsFileReadable(filename))
            return false;
        wxFileInputStream file(filename);
        if (!file.IsOk())
            return false;
        contents = ReadStream(file);
        return true;
    }

    bool SaveFile(const wxString& filename, const wxString& contents)
    {
        wxFileOutputStream file(filename);
        if (!file.IsOk())
            return false;
        wxTextOutputStream fout(file);
        fout << contents;
        return true;
    }



/*
 * LatexPreviewWindow type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LatexPreviewWindow, wxPanel )


/*
 * LatexPreviewWindow event table definition
 */

BEGIN_EVENT_TABLE( LatexPreviewWindow, wxFrame )

////@begin LatexPreviewWindow event table entries
    EVT_CLOSE( LatexPreviewWindow::OnCloseWindow )
    EVT_MENU( wxID_SAVE, LatexPreviewWindow::OnSaveClick )
    EVT_MENU( wxID_REVERT_TO_SAVED, LatexPreviewWindow::OnRevertToSavedClick )
    EVT_MENU( wxID_DEFAULT, LatexPreviewWindow::OnDefaultClick )
    EVT_MENU( wxID_OPEN, LatexPreviewWindow::OnOpenClick )
    EVT_MENU( wxID_SAVE_TPL, LatexPreviewWindow::OnSaveTplClick )
    EVT_MENU( wxID_REVERT_TO_SAVED_TPL, LatexPreviewWindow::OnRevertToSavedTplClick )
    EVT_MENU( wxID_DEFAULT_TPL, LatexPreviewWindow::OnDefaultTplClick )
    EVT_MENU( wxID_SAVE_TEX, LatexPreviewWindow::OnSaveTexClick )
    EVT_MENU( wxID_SAVEAS, LatexPreviewWindow::OnSaveasClick )
    EVT_MENU( ID_MENUITEM_FT_PNG, LatexPreviewWindow::OnMenuitemFtPngClick )
    EVT_UPDATE_UI( ID_MENUITEM_FT_PNG, LatexPreviewWindow::OnMenuitemFtPngUpdate )
    EVT_MENU( ID_MENUITEM_FT_GIF, LatexPreviewWindow::OnMenuitemFtGifClick )
    EVT_UPDATE_UI( ID_MENUITEM_FT_GIF, LatexPreviewWindow::OnMenuitemFtGifUpdate )
    EVT_MENU( ID_MENUITEM_BG_TRANSPARENT, LatexPreviewWindow::OnMenuitemBgTransparentClick )
    EVT_UPDATE_UI( ID_MENUITEM_BG_TRANSPARENT, LatexPreviewWindow::OnMenuitemBgTransparentUpdate )
    EVT_MENU( ID_MENUITEM_AUTORENDER, LatexPreviewWindow::OnMenuitemAutorenderClick )
    EVT_UPDATE_UI( ID_MENUITEM_AUTORENDER, LatexPreviewWindow::OnMenuitemAutorenderUpdate )
    EVT_MENU( wxID_EXIT, LatexPreviewWindow::OnExitClick )
    EVT_MENU( wxID_ABOUT, LatexPreviewWindow::OnAboutClick )
    EVT_NOTEBOOK_PAGE_CHANGED( ID_NOTEBOOK, LatexPreviewWindow::OnNotebookPageChanged )
    EVT_TEXT( ID_TEXTCTRL_INPUT, LatexPreviewWindow::OnTextctrlInputTextUpdated )
    EVT_BUTTON( wxID_SAVE, LatexPreviewWindow::OnSaveClick )
    EVT_BUTTON( wxID_REFRESH, LatexPreviewWindow::OnRefreshClick )
    EVT_BUTTON( wxID_CLOSE, LatexPreviewWindow::OnCloseClick )
    EVT_TEXT( ID_TEXTCTRL_TEMPLATE, LatexPreviewWindow::OnTextctrlTemplateTextUpdated )
    EVT_BUTTON( wxID_SAVE_TPL, LatexPreviewWindow::OnSaveTplClick )
    EVT_BUTTON( wxID_REVERT_TO_SAVED_TPL, LatexPreviewWindow::OnRevertToSavedTplClick )
////@end LatexPreviewWindow event table entries

END_EVENT_TABLE()


/*
 * LatexPreviewWindow constructors
 */

LatexPreviewWindow::LatexPreviewWindow()
{
    Init();
}

LatexPreviewWindow::LatexPreviewWindow(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*
 * LatexPreviewWindow creator
 */

bool LatexPreviewWindow::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
////@begin LatexPreviewWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    SetIcon(GetIconResource(wxT("icon1.xpm")));
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end LatexPreviewWindow creation

    ConfigData data;
    LoadConfig(data);

    if (data.file_type == filetype::gif)
        m_filetype = filetype::gif;
    else
        m_filetype = filetype::png;
    m_transparent = data.transparent;
    m_autorender = data.autorender;

    m_control_template->ChangeValue(data.texplate);

    m_control_input->SetFocus();
    m_control_input->ChangeValue(data.textro);

    if (data.select_start < 0)
        data.select_start = std::max(0, (int) m_control_input->GetLastPosition() + data.select_start);
    if (data.select_end < 0)
        data.select_end = std::max(data.select_start, (int) m_control_input->GetLastPosition() + data.select_end);
    m_control_input->SetSelection(data.select_start, data.select_end);

    SetSize(data.width, data.height);
    Rebuild();
    m_resize_frame = true;

    return true;
}


/*
 * LatexPreviewWindow destructor
 */

LatexPreviewWindow::~LatexPreviewWindow()
{
////@begin LatexPreviewWindow destruction
////@end LatexPreviewWindow destruction
}


/*
 * Member initialisation
 */

void LatexPreviewWindow::Init()
{
    m_make_filename = true;
    m_resize_frame = false;

    wxFileName fn = wxFileName::DirName( wxStandardPaths::Get().GetUserConfigDir() );
    fn.SetFullName(wxT(".latexpreview"));
    m_config = fn.GetFullPath();

////@begin LatexPreviewWindow member initialisation
    m_autorender = false;
    m_filetype = filetype::png;
    m_transparent = false;
    m_notebook = NULL;
    m_panel_formula = NULL;
    m_control_input = NULL;
    m_panel_image = NULL;
    m_control_image = NULL;
    m_control_template = NULL;
    m_panel_log = NULL;
    m_control_log_operation = NULL;
    m_control_log_exitcode = NULL;
    m_control_log = NULL;
////@end LatexPreviewWindow member initialisation
}

/*
 * Control creation for LatexPreviewWindow
 */

void LatexPreviewWindow::CreateControls()
{
////@begin LatexPreviewWindow content construction
    LatexPreviewWindow* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    wxMenu* itemMenu4 = new wxMenu;
    itemMenu4->Append(wxID_SAVE, _("&Save"), wxEmptyString, wxITEM_NORMAL);
    itemMenu4->Append(wxID_REVERT_TO_SAVED, _("&Revert"), wxEmptyString, wxITEM_NORMAL);
    itemMenu4->AppendSeparator();
    itemMenu4->Append(wxID_DEFAULT, _("&Default"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_MENU_FORMULA, _("&Formula"), itemMenu4);
    wxMenu* itemMenu9 = new wxMenu;
    itemMenu9->Append(wxID_OPEN, _("&Open"), wxEmptyString, wxITEM_NORMAL);
    itemMenu9->AppendSeparator();
    itemMenu9->Append(wxID_SAVE_TPL, _("&Save"), wxEmptyString, wxITEM_NORMAL);
    itemMenu9->Append(wxID_REVERT_TO_SAVED_TPL, _("&Revert"), wxEmptyString, wxITEM_NORMAL);
    itemMenu9->AppendSeparator();
    itemMenu9->Append(wxID_DEFAULT_TPL, _("&Default"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_MENU_TEMPLATE, _("&Template"), itemMenu9);
    itemMenu3->Append(wxID_SAVE_TEX, _("Save Te&X file..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->AppendSeparator();
    wxMenu* itemMenu18 = new wxMenu;
    itemMenu18->Append(wxID_SAVEAS, _("Save &as"), wxEmptyString, wxITEM_NORMAL);
    itemMenu18->AppendSeparator();
    itemMenu18->Append(ID_MENUITEM_FT_PNG, _(".&png"), wxEmptyString, wxITEM_RADIO);
    itemMenu18->Append(ID_MENUITEM_FT_GIF, _(".&gif"), wxEmptyString, wxITEM_RADIO);
    itemMenu18->AppendSeparator();
    itemMenu18->Append(ID_MENUITEM_BG_TRANSPARENT, _("&Transparent"), wxEmptyString, wxITEM_CHECK);
    itemMenu3->Append(ID_MENU_IMAGE, _("&Image"), itemMenu18);
    itemMenu3->Append(ID_MENUITEM_AUTORENDER, _("Auto refresh"), wxEmptyString, wxITEM_CHECK);
    itemMenu3->AppendSeparator();
    itemMenu3->Append(wxID_EXIT, _("&Exit"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu28 = new wxMenu;
    itemMenu28->Append(wxID_ABOUT, _("&About"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu28, _("?"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer30 = new wxBoxSizer(wxHORIZONTAL);
    itemFrame1->SetSizer(itemBoxSizer30);

    wxPanel* itemPanel31 = new wxPanel( itemFrame1, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer30->Add(itemPanel31, 1, wxGROW, 5);

    wxBoxSizer* itemBoxSizer32 = new wxBoxSizer(wxHORIZONTAL);
    itemPanel31->SetSizer(itemBoxSizer32);

    m_notebook = new wxNotebook( itemPanel31, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_TOP );

    m_panel_formula = new wxPanel( m_notebook, ID_PANEL_PREVIEW, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxVERTICAL);
    m_panel_formula->SetSizer(itemBoxSizer35);

    m_control_input = new wxTextCtrl( m_panel_formula, ID_TEXTCTRL_INPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer35->Add(m_control_input, 1, wxGROW|wxALL, 5);

    m_panel_image = new wxPanel( m_panel_formula, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    itemBoxSizer35->Add(m_panel_image, 0, wxGROW|wxALL, 5);
    wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxVERTICAL);
    m_panel_image->SetSizer(itemBoxSizer38);

    m_control_image = new wxWindow( m_panel_image, ID_FOREIGN_IMAGE_PREVIEW, wxDefaultPosition, wxSize(180, 50), wxRAISED_BORDER|wxFULL_REPAINT_ON_RESIZE );
    itemBoxSizer38->Add(m_control_image, 0, wxALIGN_CENTER_HORIZONTAL, 5);

    wxBoxSizer* itemBoxSizer40 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer35->Add(itemBoxSizer40, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton41 = new wxButton( m_panel_formula, wxID_SAVE, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(itemButton41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton42 = new wxButton( m_panel_formula, wxID_REFRESH, _("Refresh"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(itemButton42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton43 = new wxButton( m_panel_formula, wxID_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer40->Add(itemButton43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_notebook->AddPage(m_panel_formula, _("Formula"));

    wxPanel* itemPanel44 = new wxPanel( m_notebook, ID_PANEL_TEMPLATE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer45 = new wxBoxSizer(wxVERTICAL);
    itemPanel44->SetSizer(itemBoxSizer45);

    m_control_template = new wxTextCtrl( itemPanel44, ID_TEXTCTRL_TEMPLATE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
    itemBoxSizer45->Add(m_control_template, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer47 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer45->Add(itemBoxSizer47, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxButton* itemButton48 = new wxButton( itemPanel44, wxID_SAVE_TPL, _("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemButton48, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton49 = new wxButton( itemPanel44, wxID_REVERT_TO_SAVED_TPL, _("&Revert"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemButton49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton50 = new wxButton( itemPanel44, wxID_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer47->Add(itemButton50, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_notebook->AddPage(itemPanel44, _("Template"));

    m_panel_log = new wxPanel( m_notebook, ID_PANEL_LOG, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer52 = new wxBoxSizer(wxVERTICAL);
    m_panel_log->SetSizer(itemBoxSizer52);

    wxFlexGridSizer* itemFlexGridSizer53 = new wxFlexGridSizer(0, 4, 0, 0);
    itemBoxSizer52->Add(itemFlexGridSizer53, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText54 = new wxStaticText( m_panel_log, wxID_STATIC, _("Operation:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer53->Add(itemStaticText54, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_control_log_operation = new wxTextCtrl( m_panel_log, ID_TEXTCTRL_LOG_OPERATION, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemFlexGridSizer53->Add(m_control_log_operation, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText56 = new wxStaticText( m_panel_log, wxID_STATIC, _("Exit code:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer53->Add(itemStaticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_control_log_exitcode = new wxTextCtrl( m_panel_log, ID_TEXTCTRL_LOG_EXITCODE, wxEmptyString, wxDefaultPosition, wxSize(40, -1), wxTE_READONLY );
    itemFlexGridSizer53->Add(m_control_log_exitcode, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer53->AddGrowableCol(1);

    wxStaticBox* itemStaticBoxSizer58Static = new wxStaticBox(m_panel_log, wxID_ANY, _("Standard output"));
    wxStaticBoxSizer* itemStaticBoxSizer58 = new wxStaticBoxSizer(itemStaticBoxSizer58Static, wxHORIZONTAL);
    itemBoxSizer52->Add(itemStaticBoxSizer58, 1, wxGROW|wxALL, 5);
    m_control_log = new wxTextCtrl( m_panel_log, ID_TEXTCTRL_LOG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
    itemStaticBoxSizer58->Add(m_control_log, 1, wxGROW|wxALL, 5);

    m_notebook->AddPage(m_panel_log, _("Log"));

    itemBoxSizer32->Add(m_notebook, 1, wxGROW, 5);

    // Connect events and objects
    m_control_image->Connect(ID_FOREIGN_IMAGE_PREVIEW, wxEVT_SIZE, wxSizeEventHandler(LatexPreviewWindow::OnSize), NULL, this);
    m_control_image->Connect(ID_FOREIGN_IMAGE_PREVIEW, wxEVT_PAINT, wxPaintEventHandler(LatexPreviewWindow::OnPaint), NULL, this);
    m_control_image->Connect(ID_FOREIGN_IMAGE_PREVIEW, wxEVT_LEFT_DOWN, wxMouseEventHandler(LatexPreviewWindow::OnLeftDown), NULL, this);
////@end LatexPreviewWindow content construction

    m_control_image->SetBackgroundColour(*wxWHITE);
}


/*
 * Should we show tooltips?
 */

bool LatexPreviewWindow::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap LatexPreviewWindow::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin LatexPreviewWindow bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end LatexPreviewWindow bitmap retrieval
}

/*
 * Get icon resources
 */

wxIcon LatexPreviewWindow::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LatexPreviewWindow icon retrieval
    wxUnusedVar(name);
    if (name == _T("icon1.xpm"))
    {
        wxIcon icon(icon1);
        return icon;
    }
    return wxNullIcon;
////@end LatexPreviewWindow icon retrieval
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
 */

void LatexPreviewWindow::OnExitClick( wxCommandEvent& event )
{
    Close();
}




/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL
 */

void LatexPreviewWindow::OnTextctrlInputTextUpdated( wxCommandEvent& event )
{
    if (m_autorender)
        Rebuild();
}


bool LatexPreviewWindow::LoadConfig(ConfigData& data)
{
    if (!wxFileName::IsFileReadable(m_config)) {
        GetDefaultConfig(data);
        return false;
    }

    wxFileInputStream file(m_config);
    if (!file.IsOk()) {
        GetDefaultConfig(data);
        return false;
    }

    wxFileConfig config(file);

    config.Read(wxT("/config/autorender"), &data.autorender, data.autorender);
    config.Read(wxT("/window/width"), &data.width, data.width);
    config.Read(wxT("/window/height"), &data.height, data.height);

    config.Read(wxT("/document/header/tex"), &data.texplate, data.texplate);
    config.Read(wxT("/document/formula/tex"), &data.textro, data.textro);
    config.Read(wxT("/document/formula/select_start"), &data.select_start, data.select_start);
    config.Read(wxT("/document/formula/select_end"), &data.select_end, data.select_end);

    config.Read(wxT("/output/transparent"), &data.transparent, data.transparent);
    config.Read(wxT("/output/filetype"), &data.file_type, data.file_type);

    return true;
}

bool LatexPreviewWindow::SaveConfig(const ConfigData& data)
{
    wxFileConfig config;

    config.Write(wxT("/config/autorender"), data.autorender);
    config.Write(wxT("/window/width"), data.width);
    config.Write(wxT("/window/height"), data.height);

    config.Write(wxT("/document/header/tex"), data.texplate);
    config.Write(wxT("/document/formula/tex"), data.textro);
    config.Write(wxT("/document/formula/select_start"), data.select_start);
    config.Write(wxT("/document/formula/select_end"), data.select_end);

    config.Write(wxT("/output/transparent"), data.transparent);
    config.Write(wxT("/output/filetype"), data.file_type);

    wxFileOutputStream file(m_config);
    if (!file.IsOk())
        return false;
    config.Save(file);

    return true;
}

void LatexPreviewWindow::GetDefaultConfig(ConfigData& data)
{
    data.width = 400;
    data.height = 200;
    data.texplate = GetDefaultTemplate();
    data.textro = GetDefaultFormula();
    data.select_start = 0;
    data.select_end = data.textro.size();
    data.autorender = false;
    data.transparent = true;
    data.file_type = filetype::png;
}

wxString LatexPreviewWindow::GetDefaultTemplate()
{
    return wxString() +
        wxT("\\documentclass[12pt]{article}\n") +
        wxT("\\usepackage[dvips]{graphicx}\n") +
        wxT("\\usepackage{amsmath}\n") +
        wxT("\\usepackage{amssymb}\n") +
        wxT("\\pagestyle{empty}\n") +
        wxT("\\begin{document}\n\n") +
        wxT("\\begin{align*}\n") +
        insert_marker + wxT("\n") +
        wxT("\\end{align*}\n\n") +
        wxT("\\end{document}\n") +
        wxString();
}
wxString LatexPreviewWindow::GetDefaultFormula()
{
    return wxString() +
        wxT("\\int\\limits_\\mathbb{R} e^{-x^2} dx = \\sqrt{\\pi}") +
        wxString();
}

bool LatexPreviewWindow::Rebuild(bool switch_to_log)
{
    wxBitmap img;
    execution_info info;
    bool success = Build(
            m_control_input->GetValue(),
            m_control_template->GetValue(),
            img,
            info);

    m_control_log_operation->ChangeValue(info.cmd);
    m_control_log_exitcode->ChangeValue(wxString::Format(wxT("%i") , info.exitcode));
    m_control_log->ChangeValue(info.out);

    if (success) {
        SetImage(img);
        m_panel_log->Show(false);
    }
    else {
        m_panel_log->Show(true);
        if (switch_to_log)
            m_notebook->ChangeSelection(2);     // to do: => ShowLog
    }

    m_control_input->DiscardEdits();
    return success;
}

bool LatexPreviewWindow::Build(
        const wxString& text,
        const wxString& texplate,
        wxBitmap& img,
        execution_info& info )
{
    // to do: generate less files?
    wxFileName name;
    if (m_make_filename) {
        name.AssignTempFileName(wxT("latex_preview_"));
        wxRemoveFile(name.GetFullPath());
    } else
        name = m_filename;

    wxString basename = name.GetPath() + wxT("/") + name.GetName();

    int ft = m_filetype;
    bool tr = m_transparent;

    wxString file_tex = basename + wxT(".tex"),
             file_dvi = basename + wxT(".dvi"),
             file_png = basename + wxT(".png"),
             file_gif = basename + wxT(".gif"),
             file_img = ft == filetype::gif ? file_gif : file_png;

    ChangeCwd dummy(name.GetPath());

    bool success =
        BuildTex(text, texplate, file_tex, info) &&
        BuildDvi(file_tex, file_dvi, info) &&
        BuildImg(file_dvi, file_img, ft, tr, info);

    wxRemoveFile(basename + wxT(".tex"));
    wxRemoveFile(basename + wxT(".log"));
    wxRemoveFile(basename + wxT(".dvi"));
    wxRemoveFile(basename + wxT(".aux"));

    if (!success) {
        return false;
    }

    m_make_filename = false;
    m_filename = basename;
    m_filename_img = file_img;

    if (ft == filetype::gif)
        return img.LoadFile(file_gif, wxBITMAP_TYPE_GIF);
    return img.LoadFile(file_png, wxBITMAP_TYPE_PNG);
}

bool LatexPreviewWindow::BuildTex(
        const wxString& _text,
        const wxString& texplate,
        const wxString& file_tex,
        execution_info& info )
{
    info.cmd = wxT("generate .tex code");
    info.exitcode = 0;

    wxString text = _text;
    text.Trim(true);
    text.Trim(false);
    while (text.Replace(wxT(" \n"), wxT("\n"))) ;
    while (text.Replace(wxT("\n\n"), wxT("\\\\\n"))) ;

    wxString tex = texplate;
    if (tex.Replace(insert_marker, text) == 0) {
        info.out = wxT("Did not find insert marker ('") + insert_marker + wxT("') in template!");
        info.err = wxT("");
        return false;
    }

    if (!SaveFile(file_tex, tex)) {
        info.out = wxT("Failed to generate tex file: '") + file_tex + wxT("'.");
        info.err = wxT("");
    }

    return true;
}



bool LatexPreviewWindow::BuildDvi(
        const wxString& file_tex,
        const wxString& file_dvi,
        execution_info& info )
{
    std::vector<std::string> args = {
        bp::search_path("latex").string(),
        "--interaction=nonstopmode",
        to_std(file_tex)
    };

    return execute(args, info);
}


void push_back(
        std::vector<std::string>& v,
        std::initializer_list<std::string> args)
{
    v.insert(v.end(), args);
}


bool LatexPreviewWindow::BuildImg(
        const wxString& file_dvi,
        const wxString& file_img,
        int ft,
        bool transparent,
        execution_info& info )
{
    std::vector<std::string> args = {bp::search_path("dvipng").string()};

    if (m_transparent)
        push_back(args, {"-bg", "transparent"});
    else
        push_back(args, {"-bg", "rgb 1.0 1.0 1.0"});

    push_back(args, {"-Q", "10", "-T", "tight", "--follow"});

    if (m_filetype == filetype::gif)
        args.push_back("--gif");
    else
        args.push_back("--png");

    push_back(args, {"-o", to_std(file_img), to_std(file_dvi)});

    return execute(args, info);
}

void LatexPreviewWindow::SetImage(const wxBitmap& img)
{
    Freeze();

    int old_width = 0,
        old_height = 0;
    if (m_img.IsOk()) {
        old_width = m_img.GetWidth();
        old_height = m_img.GetHeight();
    }

    wxSize increase(img.GetWidth() - old_width,
                    img.GetHeight() - old_height),
           frame_size( GetSize() ),
           new_img_size(img.GetWidth() + 40, img.GetHeight() + 40);
    m_img = img;

    // m_mainpanel->Layout();

    // m_control_image->SetVirtualSizeHints( new_img_size );
    m_control_image->SetInitialSize( new_img_size );
    m_control_image->SetMinSize( new_img_size );
    m_control_image->SetSize( new_img_size );

    m_panel_formula->GetSizer()->Layout();
    GetSizer()->SetSizeHints(this);

    if (m_resize_frame) {
        if (m_control_image->GetSize().x > m_panel_image->GetSize().x)
            frame_size.x += m_control_image->GetSize().x - m_panel_image->GetSize().x;
        frame_size.y += increase.y;
    }

    // m_control_image->Refresh();
    // m_control_image->Update();
    // m_mainpanel->GetSizer()->SetSizeHints(m_mainpanel);

    SetSize(frame_size);
    Thaw();

    Refresh();
}


void LatexPreviewWindow::SetDirty()
{
    m_dirty = true;
    if (ShouldBuild())
        Rebuild();
}

bool LatexPreviewWindow::ShouldBuild()
{
    return m_dirty && m_autorender &&
        m_notebook->GetCurrentPage() == m_notebook->GetPage(0);
}


/*
 * wxEVT_LEFT_DOWN event handler for ID_FOREIGN
 */

void LatexPreviewWindow::OnLeftDown( wxMouseEvent& event )
{
    wxFileDataObject data;
    data.AddFile(m_filename_img);

    wxDropSource source(data, this);
    wxDragResult result = source.DoDragDrop(wxDrag_CopyOnly);

    if (result == wxDragCopy || result == wxDragLink)
        m_make_filename = true;
}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_PNG
 */

void LatexPreviewWindow::OnMenuitemFtPngClick( wxCommandEvent& event )
{
    m_filetype = filetype::png;
    SetDirty();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_PNG
 */

void LatexPreviewWindow::OnMenuitemFtPngUpdate( wxUpdateUIEvent& event )
{
    event.Check(m_filetype == filetype::png);
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_GIF
 */

void LatexPreviewWindow::OnMenuitemFtGifClick( wxCommandEvent& event )
{
    m_filetype = filetype::gif;
    SetDirty();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_GIF
 */

void LatexPreviewWindow::OnMenuitemFtGifUpdate( wxUpdateUIEvent& event )
{
    event.Check(m_filetype == filetype::gif);
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_BG_TRANSPARENT
 */

void LatexPreviewWindow::OnMenuitemBgTransparentClick( wxCommandEvent& event )
{
    m_transparent = event.IsChecked();
    SetDirty();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_BG_TRANSPARENT
 */

void LatexPreviewWindow::OnMenuitemBgTransparentUpdate( wxUpdateUIEvent& event )
{
    event.Check(m_transparent);
    SetDirty();
}

/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_OPEN
 */

void LatexPreviewWindow::OnOpenClick( wxCommandEvent& event )
{
    wxFileDialog dlg( this,
            wxT("Open template.."),
            wxT(""),
            wxT(""),
            wxT("*"),
            wxFD_OPEN | wxFD_FILE_MUST_EXIST );

    if (dlg.ShowModal() == wxID_OK) {
        wxString texplate;
        if (LoadFile(dlg.GetPath(), texplate))
            m_control_template->ChangeValue(texplate);
    }
}




/*
 * wxEVT_CLOSE_WINDOW event handler for ID_LATEXPREVIEWWINDOW
 */

void LatexPreviewWindow::OnCloseWindow( wxCloseEvent& event )
{
    ConfigData cfg;
    LoadConfig(cfg);

    cfg.width = GetSize().x;
    cfg.height = GetSize().y;

    cfg.autorender = m_autorender;
    cfg.transparent = m_transparent;
    cfg.file_type = m_filetype;
    SaveConfig(cfg);

    event.Skip();
}



/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CLOSE
 */

void LatexPreviewWindow::OnCloseClick( wxCommandEvent& event )
{
    Close();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE
 */

void LatexPreviewWindow::OnSaveClick( wxCommandEvent& event )
{
    ConfigData cfg;
    LoadConfig(cfg);
    cfg.textro = m_control_input->GetValue();

    long sel_start, sel_end;
    m_control_input->GetSelection(&sel_start, &sel_end);

    cfg.select_start = sel_start;
    cfg.select_end = sel_end;

    SaveConfig(cfg);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REVERT_TO_SAVED
 */

void LatexPreviewWindow::OnRevertToSavedClick( wxCommandEvent& event )
{
    ConfigData cfg;
    LoadConfig(cfg);
    m_control_input->ChangeValue(cfg.textro);
    SetDirty();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_DEFAULT
 */

void LatexPreviewWindow::OnDefaultClick( wxCommandEvent& event )
{
    m_control_input->ChangeValue(GetDefaultFormula());
    SetDirty();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE_TPL
 */

void LatexPreviewWindow::OnSaveTplClick( wxCommandEvent& event )
{
    ConfigData cfg;
    LoadConfig(cfg);
    cfg.texplate = m_control_template->GetValue();
    SaveConfig(cfg);
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_REVERT_TO_SAVED_TPL
 */

void LatexPreviewWindow::OnRevertToSavedTplClick( wxCommandEvent& event )
{
    ConfigData cfg;
    LoadConfig(cfg);
    m_control_template->ChangeValue(cfg.texplate);
    SetDirty();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_DEFAULT_TPL
 */

void LatexPreviewWindow::OnDefaultTplClick( wxCommandEvent& event )
{
    m_control_template->ChangeValue(GetDefaultTemplate());
    SetDirty();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVEAS
 */

void LatexPreviewWindow::OnSaveasClick( wxCommandEvent& event )
{
    wxFileDialog dlg( this,
            wxT("Save image to.."),
            wxT(""),
            wxFileName(m_filename_img).GetFullName(),
            wxT("*"),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

    if (dlg.ShowModal() == wxID_OK)
        wxCopyFile(m_filename_img, dlg.GetPath());
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM_AUTORENDER
 */

void LatexPreviewWindow::OnMenuitemAutorenderClick( wxCommandEvent& event )
{
    m_autorender = event.IsChecked();
    if (ShouldBuild())
        Rebuild();
}


/*
 * wxEVT_UPDATE_UI event handler for ID_MENUITEM_AUTORENDER
 */

void LatexPreviewWindow::OnMenuitemAutorenderUpdate( wxUpdateUIEvent& event )
{
    event.Check(m_autorender);
}


/*
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL_TEMPLATE
 */

void LatexPreviewWindow::OnTextctrlTemplateTextUpdated( wxCommandEvent& event )
{
    SetDirty();
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_REFRESH
 */

void LatexPreviewWindow::OnRefreshClick( wxCommandEvent& event )
{
    Rebuild(true);
}

/*
 * wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_NOTEBOOK
 */

void LatexPreviewWindow::OnNotebookPageChanged( wxNotebookEvent& event )
{
    if (ShouldBuild())
        Rebuild();
}


/*
 * wxEVT_SIZE event handler for ID_FOREIGN_IMAGE_PREVIEW
 */

void LatexPreviewWindow::OnSize( wxSizeEvent& event )
{
    // to do ...
    m_control_image->Refresh();
    event.Skip();
}


/*
 * wxEVT_PAINT event handler for ID_FOREIGN_IMAGE_PREVIEW
 */

void LatexPreviewWindow::OnPaint( wxPaintEvent& event )
{
    wxPaintDC dc(m_control_image);

    // dc.SetBackground(*wxWHITE_BRUSH);
    // dc.Clear();

    dc.DrawBitmap( m_img,
            (dc.GetSize().x - m_img.GetWidth() + 1) / 2,
            (dc.GetSize().y - m_img.GetHeight() + 1) / 2,
            true );
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_ABOUT
 */

void LatexPreviewWindow::OnAboutClick( wxCommandEvent& event )
{
    wxtk::AboutBox about(this);
    about.SetDescription( to_wx("Clever text describing the application. Not.") );
    about.SetBigIcon( GetIconResource(wxT("icon1.xpm")) );
    about.SetAppname( to_wx("wxLatexPreview\nVersion 1.2") );
    about.SetTitle( to_wx("About wxLatexPreview") );
    about.SetCopyright( to_wx("(C) 2010-2012 Thomas Gläßle") );
    about.ShowModal();
}


/*
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SAVE_TEX
 */

void LatexPreviewWindow::OnSaveTexClick( wxCommandEvent& event )
{
    wxFileName name = m_filename;
    wxString basename = name.GetPath() + wxT("/") + name.GetName();
    wxString file_tex = basename + wxT(".tex");

    wxFileDialog dlg( this,
            wxT("Save TeX file to.."),
            wxT(""),
            wxFileName(m_filename_img).GetFullName(),
            wxT("*"),
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT );

    if (dlg.ShowModal() == wxID_OK) {
        execution_info info;
        BuildTex(
            m_control_input->GetValue(),
            m_control_template->GetValue(),
            dlg.GetPath(),
            info);
    }
}

