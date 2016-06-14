
// KeyboardVisualizerDlg.h : header file
//

#pragma once

#include "Visualizer.h"

// CKeyboardVisualizerDlg dialog
class CKeyboardVisualizerDlg : public CDialogEx
{
// Construction
public:
	CKeyboardVisualizerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_KEYBOARDVISUALIZER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
public:
	Visualizer m_visualizer;
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedPauseResume();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	CScrollBar m_scrollBarBackgroundRainbow;
	CScrollBar m_scrollBarBackgroundRed;
	CScrollBar m_scrollBarBackgroundGreen;
	CScrollBar m_scrollBarBackgroundBlue;
	CScrollBar m_scrollBarVisualizerRed;
	CScrollBar m_scrollBarVisualizerGreen;
	CScrollBar m_scrollBarVisualizerBlue;
	CButton m_checkBoxRainbow;
	CButton m_checkBoxLevelColorize;
	CButton m_radioButtonPlayback;
	CButton m_radioButtonRecording;
	afx_msg void OnBnClickedLevelColorize();
	afx_msg void OnBnClickedRainbow();
	afx_msg void OnEnChangeDrawRate();
	afx_msg void OnEnChangeAmplitude();
};
