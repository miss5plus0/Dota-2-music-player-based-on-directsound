
// DsoundworkDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Dsoundwork.h"
#include "DsoundworkDlg.h"
#include "afxdialogex.h"
#include "SoundClass.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

IDirectSoundBuffer8* g_dsb8Sample1;
IDirectSoundBuffer8* g_dsb8Sample2;
SoundClass* g_scSound;
char* file1;
char* file2;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDsoundworkDlg 对话框



CDsoundworkDlg::CDsoundworkDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DSOUNDWORK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDsoundworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDsoundworkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDsoundworkDlg::OnBnClickedButton1)
//	ON_EN_CHANGE(IDC_EDIT1, &CDsoundworkDlg::OnEnChangeEdit1)
ON_BN_CLICKED(IDC_BUTTON3, &CDsoundworkDlg::OnBnClickedButton3)
ON_BN_CLICKED(IDC_BUTTON2, &CDsoundworkDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDsoundworkDlg 消息处理程序

BOOL CDsoundworkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDsoundworkDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDsoundworkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		CPaintDC   dc(this);
		CRect   rect;
		GetClientRect(&rect);    //获取对话框长宽      
		CDC   dcBmp;             //定义并创建一个内存设备环境
		dcBmp.CreateCompatibleDC(&dc);             //创建兼容性DC
		CBitmap   bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP2);    //载入资源中图片
		BITMAP   m_bitmap;                         //图片变量               
		bmpBackground.GetBitmap(&m_bitmap);       //将图片载入位图中
												  //将位图选入临时内存设备环境
		CBitmap  *pbmpOld = dcBmp.SelectObject(&bmpBackground);
		//调用函数显示图片StretchBlt显示形状可变
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcBmp, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDsoundworkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDsoundworkDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	HWND hWnd;
	DWORD dwFirstTick;

	g_dsb8Sample1 = 0;
	g_dsb8Sample2 = 0;
	g_scSound = new SoundClass();

	hWnd = this->m_hWnd;
	srand((unsigned)time(NULL));
	double ran1 = rand() / double(RAND_MAX);
	double ran2 = rand() / double(RAND_MAX);
	if (ran1 >= 0.5) file1 = "ShiftingSnows.wav";
	else file1 = "Wailing Mountains.wav";
	if (ran2 >= 0.5) file2 = "ShiftingSnows.wav";
	else file2 = "Wailing Mountains.wav";
	if (!g_scSound->Initialize(hWnd))
		SetDlgItemText(IDC_STATIC, (CString)"播放失败");

	if (!g_scSound->LoadWaveFile(file1, &g_dsb8Sample1))
		SetDlgItemText(IDC_STATIC, (CString)"播放失败");
	if (!g_scSound->LoadWaveFile(file2, &g_dsb8Sample2))
		SetDlgItemText(IDC_STATIC, (CString)"播放失败");

	if (!g_scSound->PlayWaveFile(&g_dsb8Sample1, 0, DSBVOLUME_MAX))
		SetDlgItemText(IDC_STATIC, (CString)"播放失败");
	else SetDlgItemText(IDC_STATIC, (CString)"第一首");


	//dwFirstTick = GetTickCount();
	//while (GetTickCount() - dwFirstTick < 3000);

	
	dwFirstTick = GetTickCount();
	while (GetTickCount() - dwFirstTick < 10000);
	g_dsb8Sample1->Stop();

	if (!g_scSound->PlayWaveFile(&g_dsb8Sample2, 0, DSBVOLUME_MAX))
		SetDlgItemText(IDC_STATIC, (CString)"播放失败");
	else SetDlgItemText(IDC_STATIC, (CString)"第二首");
	dwFirstTick = GetTickCount();
	while (GetTickCount() - dwFirstTick < 10000);
	g_dsb8Sample2->Stop();
	SetDlgItemText(IDC_STATIC, (CString)"第一首");
	g_dsb8Sample1->Play(0,0,0);
	dwFirstTick = GetTickCount();
	while (GetTickCount() - dwFirstTick < 10000);
	g_scSound->ShutdownWaveFile(&g_dsb8Sample1);
	SetDlgItemText(IDC_STATIC, (CString)"第二首");
	g_dsb8Sample2->Play(0, 0, 0);
	dwFirstTick = GetTickCount();
	while (GetTickCount() - dwFirstTick < 10000);
	g_scSound->ShutdownWaveFile(&g_dsb8Sample2);
	g_scSound->Shutdown();

}




void CDsoundworkDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (*file1 == *file2) {
		IDirectSoundBuffer8* g_dsb8result1;
		SoundClass* resultSound = new SoundClass();
		HWND hWnd = this->m_hWnd;
		if (!resultSound->Initialize(hWnd))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		if (!resultSound->LoadWaveFile("god like.wav", &g_dsb8result1));
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
	    if (!resultSound->PlayWaveFile(&g_dsb8result1, 0, DSBVOLUME_MAX))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		SetDlgItemText(IDC_STATIC, (CString)"正确");
		DWORD dwFirstTick;
		dwFirstTick = GetTickCount();
		while (GetTickCount() - dwFirstTick < 2000);
		resultSound->ShutdownWaveFile(&g_dsb8result1);
	}
	else {
		IDirectSoundBuffer8* g_dsb8result2;
		SoundClass* resultSound = new SoundClass();
		HWND hWnd = this->m_hWnd;
		if (!resultSound->Initialize(hWnd))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		
		if (!resultSound->LoadWaveFile("first blood.wav", &g_dsb8result2))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		if (!resultSound->PlayWaveFile(&g_dsb8result2, 0, DSBVOLUME_MAX))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		SetDlgItemText(IDC_STATIC, (CString)"错误");
		DWORD dwFirstTick;
		dwFirstTick = GetTickCount();
		while (GetTickCount() - dwFirstTick < 2000);
		resultSound->ShutdownWaveFile(&g_dsb8result2);
	}
}


void CDsoundworkDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (*file1 != *file2) {
		IDirectSoundBuffer8* g_dsb8result1;
		SoundClass* resultSound = new SoundClass();
		HWND hWnd = this->m_hWnd;
		if (!resultSound->Initialize(hWnd))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");

		if (!resultSound->LoadWaveFile("god like.wav", &g_dsb8result1))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		if (!resultSound->PlayWaveFile(&g_dsb8result1, 0, DSBVOLUME_MAX))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		SetDlgItemText(IDC_STATIC, (CString)"正确");
		DWORD dwFirstTick;
		dwFirstTick = GetTickCount();
		while (GetTickCount() - dwFirstTick < 2000);
		resultSound->ShutdownWaveFile(&g_dsb8result1);
	}
	else {
		IDirectSoundBuffer8* g_dsb8result2;
		SoundClass* resultSound = new SoundClass();
		HWND hWnd = this->m_hWnd;
		if (!resultSound->Initialize(hWnd))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");

		if (!resultSound->LoadWaveFile("first blood.wav", &g_dsb8result2))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		if (!resultSound->PlayWaveFile(&g_dsb8result2, 0, DSBVOLUME_MAX))
			SetDlgItemText(IDC_STATIC, (CString)"播放失败");
		SetDlgItemText(IDC_STATIC, (CString)"错误");
		DWORD dwFirstTick;
		dwFirstTick = GetTickCount();
		while (GetTickCount() - dwFirstTick < 2000);
		resultSound->ShutdownWaveFile(&g_dsb8result2);
	}
}
