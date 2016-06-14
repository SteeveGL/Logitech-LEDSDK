
// KeyboardVisualizerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyboardVisualizer.h"
#include "KeyboardVisualizerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RGB_MIN 0
#define RGB_MAX 255
#define RGB_PERCENT_MAX 100
#define ALPHA_MAX 255

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeyboardVisualizerDlg dialog
CKeyboardVisualizerDlg::CKeyboardVisualizerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKeyboardVisualizerDlg::IDD, pParent), m_visualizer()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_visualizer.Init();
}

void CKeyboardVisualizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, SCROLL_BACKGROUND_RAINBOW, m_scrollBarBackgroundRainbow);
	DDX_Control(pDX, SCROLL_BACKGROUND_RED, m_scrollBarBackgroundRed);
	DDX_Control(pDX, SCROLL_BACKGROUND_GREEN, m_scrollBarBackgroundGreen);
	DDX_Control(pDX, SCROLL_BACKGROUND_BLUE, m_scrollBarBackgroundBlue);
	DDX_Control(pDX, SCROLL_VISUALIZER_RED, m_scrollBarVisualizerRed);
	DDX_Control(pDX, SCROLL_VISUALIZER_GREEN, m_scrollBarVisualizerGreen);
	DDX_Control(pDX, SCROLL_VISUALIZER_BLUE, m_scrollBarVisualizerBlue);
	DDX_Control(pDX, CHECK_RAINBOW, m_checkBoxRainbow);
	DDX_Control(pDX, CHECK_LEVEL_COLORIZE, m_checkBoxLevelColorize);
	DDX_Control(pDX, RADIO_PLAYBACK, m_radioButtonPlayback);
	DDX_Control(pDX, RADIO_RECORDING, m_radioButtonRecording);
}

BEGIN_MESSAGE_MAP(CKeyboardVisualizerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(BUTTON_START, &CKeyboardVisualizerDlg::OnBnClickedStart)
	ON_BN_CLICKED(BUTTON_STOP, &CKeyboardVisualizerDlg::OnBnClickedStop)
	ON_BN_CLICKED(BUTTON_PAUSE_RESUME, &CKeyboardVisualizerDlg::OnBnClickedPauseResume)
	ON_BN_CLICKED(CHECK_LEVEL_COLORIZE, &CKeyboardVisualizerDlg::OnBnClickedLevelColorize)
	ON_BN_CLICKED(CHECK_RAINBOW, &CKeyboardVisualizerDlg::OnBnClickedRainbow)
	ON_EN_CHANGE(EDIT_DRAW_RATE, &CKeyboardVisualizerDlg::OnEnChangeDrawRate)
	ON_EN_CHANGE(EDIT_AMPLITUDE, &CKeyboardVisualizerDlg::OnEnChangeAmplitude)
END_MESSAGE_MAP()


// CKeyboardVisualizerDlg message handlers

BOOL CKeyboardVisualizerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize our GUI elements
	SetDlgItemInt(EDIT_DRAW_RATE, DEFAULT_REDRAW_MS, 0);
	SetDlgItemInt(EDIT_AMPLITUDE, DEFAULT_AMPTITUDE, 0);

	m_checkBoxRainbow.SetCheck(DEFAULT_IS_RAINBOW);
	m_checkBoxLevelColorize.SetCheck(DEFAULT_IS_COLORIZED);
	m_radioButtonRecording.SetCheck(1);

	m_scrollBarBackgroundRainbow.SetScrollRange(0, 100);
	m_scrollBarBackgroundRainbow.SetScrollPos(DEFAULT_BACKGROUND_RAINBOW);
	m_scrollBarBackgroundRed.SetScrollRange(0, 100);
	m_scrollBarBackgroundRed.SetScrollPos(DEFAULT_BACKGROUND_RED);
	m_scrollBarBackgroundGreen.SetScrollRange(0, 100);
	m_scrollBarBackgroundGreen.SetScrollPos(DEFAULT_BACKGROUND_GREEN);
	m_scrollBarBackgroundBlue.SetScrollRange(0, 100);
	m_scrollBarBackgroundBlue.SetScrollPos(DEFAULT_BACKGROUND_BLUE);

	m_scrollBarVisualizerRed.SetScrollRange(0, 100);
	m_scrollBarVisualizerRed.SetScrollPos(DEFAULT_VISUALIZER_RED);
	m_scrollBarVisualizerGreen.SetScrollRange(0, 100);
	m_scrollBarVisualizerGreen.SetScrollPos(DEFAULT_VISUALIZER_GREEN);
	m_scrollBarVisualizerBlue.SetScrollRange(0, 100);
	m_scrollBarVisualizerBlue.SetScrollPos(DEFAULT_VISUALIZER_BLUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeyboardVisualizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyboardVisualizerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKeyboardVisualizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CKeyboardVisualizerDlg::OnBnClickedStart()
{
	m_visualizer.Start();
}


void CKeyboardVisualizerDlg::OnBnClickedStop()
{
	m_visualizer.Stop();
}


void CKeyboardVisualizerDlg::OnBnClickedPauseResume()
{
	if (m_visualizer.isActive) {
		if (m_visualizer.isPaused) {
			m_visualizer.Resume();

			HWND hButton = ::GetDlgItem(m_hWnd, BUTTON_PAUSE_RESUME);
			::SetWindowText(hButton, _T("Pause"));
		}
		else {
			m_visualizer.Pause();

			HWND hButton = ::GetDlgItem(m_hWnd, BUTTON_PAUSE_RESUME);
			::SetWindowText(hButton, _T("Resume"));
		}
	}
}

void CKeyboardVisualizerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int CurPos = pScrollBar->GetScrollPos();

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		CurPos = RGB_MIN;
		break;

	case SB_RIGHT:      // Scroll to far right.
		CurPos = RGB_PERCENT_MAX;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;

	case SB_LINELEFT:      // Scroll left.
		if (CurPos > RGB_MIN)
			CurPos--;
		break;

	case SB_LINERIGHT:   // Scroll right.
		if (CurPos < RGB_PERCENT_MAX)
			CurPos++;
		break;

	case SB_PAGELEFT:    // Scroll one page left.
	{
		// Get the page size. 
		SCROLLINFO   info;
		pScrollBar->GetScrollInfo(&info, SIF_ALL);

		if (CurPos > RGB_MIN)
			CurPos = max(0, CurPos - (int)info.nPage);
	}
	break;

	case SB_PAGERIGHT:      // Scroll one page right
	{
		// Get the page size. 
		SCROLLINFO   info;
		pScrollBar->GetScrollInfo(&info, SIF_ALL);

		if (CurPos < RGB_PERCENT_MAX)
			CurPos = min(122, CurPos + (int)info.nPage);
	}
	break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		CurPos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		CurPos = nPos;     // position that the scroll box has been dragged to.
		break;
	}

	pScrollBar->SetScrollPos(CurPos);

	int backgroundRainbow = m_scrollBarBackgroundRainbow.GetScrollPos();
	int backgroundRed = m_scrollBarBackgroundRed.GetScrollPos();
	int backgroundGreen = m_scrollBarBackgroundGreen.GetScrollPos();
	int backgroundBlue = m_scrollBarBackgroundBlue.GetScrollPos();

	int visualizerRed = m_scrollBarVisualizerRed.GetScrollPos();
	int visualizerGreen = m_scrollBarVisualizerGreen.GetScrollPos();
	int visualizerBlue = m_scrollBarVisualizerBlue.GetScrollPos();

	m_visualizer.keyboardWriter.backgroundRainbow = backgroundRainbow;
	m_visualizer.keyboardWriter.backgroundRed = backgroundRed;
	m_visualizer.keyboardWriter.backgroundGreen = backgroundGreen;
	m_visualizer.keyboardWriter.backgroundBlue = backgroundBlue;

	m_visualizer.keyboardWriter.visualizerRed = visualizerRed;
	m_visualizer.keyboardWriter.visualizerGreen = visualizerGreen;
	m_visualizer.keyboardWriter.visualizerBlue = visualizerBlue;

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CKeyboardVisualizerDlg::OnBnClickedLevelColorize()
{
	CWnd *wndRed = this->GetDlgItem(SCROLL_VISUALIZER_RED);
	CWnd *wndGreen = this->GetDlgItem(SCROLL_VISUALIZER_GREEN);
	CWnd *wndBlue = this->GetDlgItem(SCROLL_VISUALIZER_BLUE);

	if (wndRed->IsWindowEnabled()) {
		m_visualizer.keyboardWriter.isColorized = true;

		wndRed->EnableWindow(FALSE);
		wndGreen->EnableWindow(FALSE);
		wndBlue->EnableWindow(FALSE);
	}
	else {
		m_visualizer.keyboardWriter.isColorized = false;

		wndRed->EnableWindow(TRUE);
		wndGreen->EnableWindow(TRUE);
		wndBlue->EnableWindow(TRUE);
	}
}

void CKeyboardVisualizerDlg::OnBnClickedRainbow()
{
	CWnd *wndRainbow = this->GetDlgItem(SCROLL_BACKGROUND_RAINBOW);
	CWnd *wndRed = this->GetDlgItem(SCROLL_BACKGROUND_RED);
	CWnd *wndGreen = this->GetDlgItem(SCROLL_BACKGROUND_GREEN);
	CWnd *wndBlue = this->GetDlgItem(SCROLL_BACKGROUND_BLUE);

	if (wndRainbow->IsWindowEnabled()) {
		m_visualizer.keyboardWriter.isRainbow = false;

		wndRainbow->EnableWindow(FALSE);
		wndRed->EnableWindow(TRUE);
		wndGreen->EnableWindow(TRUE);
		wndBlue->EnableWindow(TRUE);
	}
	else {
		m_visualizer.keyboardWriter.isRainbow = true;

		wndRainbow->EnableWindow(TRUE);
		wndRed->EnableWindow(FALSE);
		wndGreen->EnableWindow(FALSE);
		wndBlue->EnableWindow(FALSE);
	}
}


void CKeyboardVisualizerDlg::OnEnChangeDrawRate()
{
	int drawRateMS = GetDlgItemInt(EDIT_DRAW_RATE, 0, 0);

	m_visualizer.redrawMS = drawRateMS;
}


void CKeyboardVisualizerDlg::OnEnChangeAmplitude()
{
	int amplitude = GetDlgItemInt(EDIT_AMPLITUDE, 0, 0);

	m_visualizer.amplitude = amplitude;
}
