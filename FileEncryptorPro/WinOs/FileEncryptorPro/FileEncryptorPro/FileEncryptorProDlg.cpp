
// FileEncryptorProDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileEncryptorPro.h"
#include "FileEncryptorProDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFileEncryptorProDlg 对话框



CFileEncryptorProDlg::CFileEncryptorProDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileEncryptorProDlg::IDD, pParent)
	, m_str_Password(_T(""))
	, m_str_SavePath(_T(""))
	, m_str_OpenFile(_T(""))
	, m_isRunning(false)
	, m_pEncrypteInfo(NULL)
	, m_str_process_info(_T("拖动文件到本窗口添加文件，支持拖入文件夹加密，加密文件后缀为.flcl，请牢记密码 ,点击F9查看帮助"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileEncryptorProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WORK_LIST, m_lb_WorkList);
	DDX_Control(pDX, IDC_BUTTON_START_WORK, m_btn_StartWork);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_ALL_ITEM, m_btn_ClearAllItem);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_SELECT_ITEM, m_btn_RemoveSelectItem);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_str_Password);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_SAVE_PATH, m_str_SavePath);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_OPEN_FILE, m_str_OpenFile);
	DDX_Control(pDX, IDC_BUTTON_OPEN_FILE, m_btn_OpenFile);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_OPEN_FILE, m_edit_OpenFile);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_SAVE_PATH, m_edit_SavePath);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edit_Password);
	DDX_Control(pDX, IDC_CHECK_NOT_ADD_SUFFIX, m_ckbtn_AddDefSuffix);
	DDX_Control(pDX, IDC_CHECK_DELETE_SOURCE_FILE, m_ckbtn_DeleteSourceFile);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_pro_Process);
	DDX_Text(pDX, IDC_STATIC_PROCESS_INFO, m_str_process_info);
}

BEGIN_MESSAGE_MAP(CFileEncryptorProDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFileEncryptorProDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CFileEncryptorProDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SAVE_PATH, &CFileEncryptorProDlg::OnBnClickedButtonOpenSavePath)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_SELECT_ITEM, &CFileEncryptorProDlg::OnBnClickedButtonRemoveSelectItem)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL_ITEM, &CFileEncryptorProDlg::OnBnClickedButtonClearAllItem)
	ON_BN_CLICKED(IDC_BUTTON_START_WORK, &CFileEncryptorProDlg::OnBnClickedButtonStartWork)
	ON_WM_DROPFILES()
	ON_MESSAGE(UM_THREAD_PROCESS_INFO, EncryptMsgProc)
END_MESSAGE_MAP()


// CFileEncryptorProDlg 消息处理程序

BOOL CFileEncryptorProDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	InitControls();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileEncryptorProDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileEncryptorProDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileEncryptorProDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileEncryptorProDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
}
/*初始化控件*/
void CFileEncryptorProDlg::InitControls()
{
	((CButton*)GetDlgItem(IDC_RADIO_ENCRYPTE))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_DECRYTE))->SetCheck(0);
	m_pro_Process.SetRange(0,100);
	m_pro_Process.SetStep(1);
	m_lb_WorkList.SetHorizontalExtent(U_MAX_PATH_SIZE);
}

/*添加选择文件到列表框*/
void CFileEncryptorProDlg::OnBnClickedButtonOpenFile()
{
	AddOpenFileToList();
}

/*打开保存文件夹，用系统方式*/
void CFileEncryptorProDlg::OnBnClickedButtonOpenSavePath()
{
	UpdateData(TRUE);
	ShellExecute(NULL, TEXT("open"), m_str_SavePath, NULL, NULL, SW_SHOW);
}

/*删除选中项*/
void CFileEncryptorProDlg::OnBnClickedButtonRemoveSelectItem()
{
	RemoveSelectListItem();
}

/*清空列表框内容*/
void CFileEncryptorProDlg::OnBnClickedButtonClearAllItem()
{
	m_lb_WorkList.ResetContent();
}


/*移除选中的列表框项*/
void CFileEncryptorProDlg::RemoveSelectListItem()
{
	int selIndex = m_lb_WorkList.GetCurSel();
	if (selIndex == -1)
		return;
	m_lb_WorkList.DeleteString(selIndex);
}


/*添加一个路径到列表框*/
void CFileEncryptorProDlg::AddOnePathToList(TCHAR * path)
{
	//判断要添加的文件是否已经存在列表框中
	bool exist = false;
	int count = m_lb_WorkList.GetCount();
	CString str;
	for (int i = 0; i < count; i++)
	{
		m_lb_WorkList.GetText(i, str);
		if (str == path)
		{
			exist = true;
			break;
		}
	}
	//如果不存在列表框中则添加
	if (exist == false)
	{
		//获得文件属性，没有属性则文件或文件夹不存在
		DWORD dwAttr = GetFileAttributes(path);
		if (dwAttr == -1)
			return;
		//判断是文件还是文件夹
		if ((dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
		{
			m_lb_WorkList.AddString(path);
		}
		else if ((dwAttr&FILE_ATTRIBUTE_DIRECTORY))
		{
			//如果要添加文件夹，给出提示，同意后添加
			CString temp;
			temp.Format(TEXT("检测到添加文件夹:\n\n\t%s\n\n\t【注意】处理文件夹是非常耗时的操作，特别这个文件夹特别大时\n\n\t是否确认导入此文件夹？"), path);
			if (MessageBox(temp, NULL, MB_OKCANCEL) == IDOK)
				m_lb_WorkList.AddString(path);
		}
	}
}
/*文件拖入处理*/
void CFileEncryptorProDlg::OnDropFiles(HDROP hDropInfo)
{
	//拖入文件时激活窗口，窗口置前，提供更好的体验
	this->SetActiveWindow();
	this->SetForegroundWindow();
	//遍历拖入文件并添加到列表框
	int nDropCount = DragQueryFile(hDropInfo, -1, NULL, 0);
	for (int i = 0; i< nDropCount; i++)
	{
		WCHAR szPath[MAX_PATH];
		DragQueryFile(hDropInfo, i, szPath, U_MAX_PATH_SIZE);
		AddOnePathToList(szPath);
	}
	DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


/*前置消息处理，这里用作按键处理*/
BOOL CFileEncryptorProDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)//软件内全局按键
	{
		switch (pMsg->wParam)
		{
		case VK_F9://F9获得此帮助信息
			MessageBox(TEXT("操作提示：\n\
\t添加文件可以选取添加，也可以拖入文件\n\
\t回车键可以自动进入下一个输入框\n\
\t上下方向键可以来回切换输入框\n\
\t左右方向键可以切换加解密方式\n\
\tDelete键可以删除输入框中的 内容\n\
\tHome键可以勾选取消使用默认后缀\n\
\tEnd键可以勾选取消删除源文件\n\
\tF12键可以开始运行，和点击开始按钮一样\n\
\tF4键可以清除列表框，和清除按钮一样\n\
\n\t\tDev: Ugex.Savelar\
"),0,MB_OK|MB_ICONINFORMATION);

			break;
		}
	}
	if (m_isRunning)
	{
		return CDialogEx::PreTranslateMessage(pMsg);
	}
	if (pMsg->hwnd == m_lb_WorkList.GetSafeHwnd())//列表框按键
	{
		int selIndex = m_lb_WorkList.GetCurSel();
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_DELETE://Delete键删除当前选中项
				RemoveSelectListItem();
				break;
			case VK_UP://上下键切换当前选中项
				m_lb_WorkList.SetCurSel(selIndex + 1);
				break;
			case VK_DOWN:
				m_lb_WorkList.SetCurSel(selIndex - 1 < -1 ? -1 : selIndex - 1);
				break;
			}
		}
	}
	if (pMsg->hwnd == m_edit_OpenFile.GetSafeHwnd())//文件输入框按键
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_DELETE://Delete键清空文件选框
				m_str_OpenFile = "";
				UpdateData(FALSE);
				break;
			case VK_RETURN://回车键添加文件输入框内容到列表框
				AddOpenFileToList();
				break;
			case VK_UP://方向上下键切换输入框
				m_edit_Password.SetFocus();
				break;
			case VK_DOWN:
				m_edit_SavePath.SetFocus();
				break;
			}
		}
	}
	if (pMsg->hwnd == m_edit_SavePath.GetSafeHwnd())//保存路径输入框按键
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_DELETE://Delete键清空保存路径框
				m_str_SavePath = "";
				UpdateData(FALSE);
				break;
			case VK_RETURN://回车键输入光标切换到密码输入框
				m_edit_Password.SetFocus();
				break;
			case VK_UP://方向上下键切换输入框
				m_edit_OpenFile.SetFocus();
				break;
			case VK_DOWN:
				m_edit_Password.SetFocus();
				break;
			}
		}
	}
	if (pMsg->hwnd == m_edit_Password.GetSafeHwnd())//密码输入框按键
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_DELETE://Delete键清空密码框
				m_str_Password = "";
				UpdateData(FALSE);
				break;
			case VK_RETURN://回车键激活开始按钮，但是没有作用
				m_btn_StartWork.SetActiveWindow();
				break;
			case VK_UP://方向上下键切换输入框
				m_edit_SavePath.SetFocus();
				break;
			case VK_DOWN:
				m_edit_OpenFile.SetFocus();
				break;
			}
		}
	}
	if (pMsg->message == WM_KEYDOWN)//应用内全局按键
	{
		switch (pMsg->wParam)
		{
		case VK_F12://F12开始处理，和开始按钮一样
			StartWorkProcess();
			break;
		case VK_F4://F4清空列表框，和清空按钮一样
			m_lb_WorkList.ResetContent();
			break;
		case VK_LEFT://方向左键选中加密单选框
			((CButton*)GetDlgItem(IDC_RADIO_ENCRYPTE))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_DECRYTE))->SetCheck(0);
			break;
		case VK_RIGHT://方向右键选中解密单选框
			((CButton*)GetDlgItem(IDC_RADIO_ENCRYPTE))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_DECRYTE))->SetCheck(1);
			break;
		case VK_HOME://Home键选中、取消不使用默认后缀复选框
			if (IsDlgButtonChecked(IDC_CHECK_NOT_ADD_SUFFIX) == 1)
				((CButton*)GetDlgItem(IDC_CHECK_NOT_ADD_SUFFIX))->SetCheck(0);
			else
				((CButton*)GetDlgItem(IDC_CHECK_NOT_ADD_SUFFIX))->SetCheck(1);
			break;
		case VK_END://End键选中、取消删除源文件复选框
			if (IsDlgButtonChecked(IDC_CHECK_DELETE_SOURCE_FILE) == 1)
				((CButton*)GetDlgItem(IDC_CHECK_DELETE_SOURCE_FILE))->SetCheck(0);
			else
				((CButton*)GetDlgItem(IDC_CHECK_DELETE_SOURCE_FILE))->SetCheck(1);
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

/*添加打开文件输入框到列表*/
void CFileEncryptorProDlg::AddOpenFileToList()
{
	UpdateData(TRUE);
	m_str_OpenFile = m_str_OpenFile.Trim();//去除首尾空格
	AddOnePathToList(m_str_OpenFile.GetBuffer());
	m_str_OpenFile = "";//输入框置空
	UpdateData(FALSE);
}
/*获得文件名起始下标从路径*/
int GetFileNameIndexFromPath(char * path,char pathSpliter)
{
	int index = strlen(path)-1;
	while (index > 0 && path[index] != pathSpliter)
		index--;
	return index+1;
}
/*获得文件后缀开始下标从路径*/
int GetSuffixIndexFromPath(char * path, char pathSpliter)
{
	int index = strlen(path) - 1;
	while (index > 0 && path[index] != pathSpliter && path[index]!='.')
		index--;
	return index;
}
/*字符串转换为大写*/
void StringToUpper(char * str)
{
	while (*str)
	{
		if (*str >= 'a'&&*str <= 'z')
			*str &= ~32;
		str++;
	}
	
}
/*字符串转换为小写*/
void StringToLower(char * str)
{
	while (*str)
	{
		if (*str >= 'A'&&*str <= 'Z')
			*str |= 32;
		str++;
	}
}
/*创建多级目录UNICODE版，注释见ASCII版*/
void CreateMultiDirectoryW(WCHAR * path, WCHAR pathSpliter)
{
	WCHAR temp[U_MAX_PATH_SIZE] = { 0 };
	int index = 0;
	while (path[index])
	{
		memset(temp, 0, sizeof(temp));
		while (path[index] && path[index]!=pathSpliter)
			index++;
		memcpy(temp, path, index*sizeof(WCHAR));
		CreateDirectoryW(temp,NULL);
		index++;
	}
}
/*创建多级目录ASCII版*/
void CreateMultiDirectoryA(char * path, char pathSpliter)
{
	//临时保存创建的路径
	char temp[U_MAX_PATH_SIZE] = { 0 };
	int index = 0;
	while (path[index])
	{
		//清空临时路径
		memset(temp, 0, sizeof(temp));
		while (path[index] && path[index] != pathSpliter)//直到获取到下一个路径分隔符或者结束位置
			index++;
		memcpy(temp, path, index*sizeof(char));//拷贝目前要创建的路径
		CreateDirectoryA(temp, NULL);//创建路径
		index++;//跳过这一个路径分隔符
	}
}
void CFileEncryptorProDlg::OnBnClickedButtonStartWork()
{
	StartWorkProcess();
}
/*文件夹处理函数*/
void EncryptDirectoryProc(PEncrypteInfomation info,PProcessInfo pinfo,char * path)
{
	//备份保存路径
	char szParentPath[U_MAX_PATH_SIZE];
	strcpy(szParentPath, path);
	//准备遍历目录数据
	WIN32_FIND_DATAA FindFileData;
	HANDLE hListFile;
	//构造遍历文件目标问所有
	char szFilePath[U_MAX_PATH_SIZE];
	strcpy(szFilePath, szParentPath);
	strcat(szFilePath, "\\*");
	//开始遍历并获得遍历句柄
	hListFile = FindFirstFileA(szFilePath, &FindFileData);
	//如果遍历开始成功则执行遍历
	if (hListFile != INVALID_HANDLE_VALUE)
	{
		//定义常量，路径分隔符和默认后缀
		static const char pathSpliter = '\\';
		static const char lockFileSuffix[] = { ".flcl" };
		//创建新的目标路径
		CreateMultiDirectoryA(info->szSavePath, pathSpliter);
		//创建目标文件源文件存储
		static char szSrcFileName[U_MAX_PATH_SIZE] = { 0 };
		static char szDstFileName[U_MAX_PATH_SIZE] = { 0 };
		do
		{
			if (!strcmp(FindFileData.cFileName, ".") == 0 &&
				!strcmp(FindFileData.cFileName, "..") == 0 )//避免循环遍历，过滤掉本路径和父路径
			{
				//获取遍历到文件的完整路径
				sprintf(szSrcFileName, "%s%c%s\0", szParentPath, pathSpliter, FindFileData.cFileName);
				//开始构造目标路径
				int fni = GetFileNameIndexFromPath(szSrcFileName, pathSpliter);
				if (info->bisEncrypte)
				{
					sprintf(szDstFileName, "%s%c%s%s\0", info->szSavePath, pathSpliter, &szSrcFileName[fni], (info->bisNoSuffix ? "" : lockFileSuffix));
				}
				else
				{
					sprintf(szDstFileName, "%s%c%s\0", info->szSavePath, pathSpliter, &szSrcFileName[fni]);
					int sfi = GetSuffixIndexFromPath(szDstFileName, pathSpliter);
					char suffix[20] = { 0 };
					if (strlen(&szDstFileName[sfi]) < 20)//防止一些后缀过长的，直接跳过即可，不可能是默认后缀
					{
						strcpy(suffix, &szDstFileName[sfi]);
						StringToLower(suffix);
						if (strcmp(suffix, lockFileSuffix) == 0)
						{
							szDstFileName[sfi] = 0;
						}
					}
					
				}
				//填充处理信息
				sprintf(pinfo->szSrcFileName, "%s\0", &szSrcFileName[fni]);
				sprintf(pinfo->szDstFileName, "%s\0", &szDstFileName[GetFileNameIndexFromPath(szDstFileName, pathSpliter)]);
				//是否是文件夹
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					//文件夹构造新的处理信息进行递归
					EncrypteInfomation nextDirInfo = { 0 };
					memcpy(&nextDirInfo, info, sizeof(EncrypteInfomation));
					strcpy(nextDirInfo.szSavePath, szDstFileName);
					EncryptDirectoryProc(&nextDirInfo,pinfo, szSrcFileName);
				}
				else
				{
					//根据操作类型处理文件，获取处理结果
					bool ret = true;
					if (info->bisEncrypte)
						ret = info->locker->lock(szSrcFileName, szDstFileName, info->szPassword);
					else
						ret = info->locker->unlock(szSrcFileName, szDstFileName, info->szPassword);
					//填充处理信息
					if (ret == true)
					{
						pinfo->nSuccessCount++;
						pinfo->bisSuccess = true;
					}
					else
					{
						pinfo->nFailureCount++;
						pinfo->bisSuccess = false;
					}
					//由于递归遍历，一开始没有获取总共的文件数量，因此这里增加
					pinfo->nProcessCount++;
					//如果需要删除源文件则删除
					if (info->bisDelSource && ret)
						DeleteFileA(szSrcFileName);
					//发送处理信心消息
					SendMessage(info->hWnd, UM_THREAD_PROCESS_INFO, 
						(WPARAM)(int)((pinfo->nSuccessCount+pinfo->nFailureCount)*1.0/pinfo->nProcessCount*100), 
						(LPARAM)pinfo);
				}

			}
			//进行下一次遍历
		} while (FindNextFileA(hListFile, &FindFileData));
	}
}
/*
处理线程函数
*/
DWORD WINAPI EncryptThreadProc( LPVOID lpThreadParameter )
{
	//定义路径分隔符和默认后缀
	const char pathSpliter = '\\';
	const char lockFileSuffix[] = { ".flcl" };
	//转换指针并创建目标路径
	PEncrypteInfomation info = (PEncrypteInfomation)lpThreadParameter;
	CreateMultiDirectoryA(info->szSavePath, pathSpliter);
	USES_CONVERSION;
	//定义源文件目标文件路径存放数组
	char szSrcFileName[U_MAX_PATH_SIZE] = { 0 };
	char szDstFileName[U_MAX_PATH_SIZE] = { 0 };
	//获得列表框项数
	int native_list_count = info->pWorkList->GetCount();
	//定义处理信息并初始化
	static ProcessInfo pinfo = { 0 };
	pinfo.nSuccessCount = 0;
	pinfo.nFailureCount = 0;
	pinfo.nProcessCount = native_list_count;
	//发送开始处理消息之后开始处理
	SendMessage(info->hWnd, UM_THREAD_PROCESS_INFO, (WPARAM)0, (LPARAM)&pinfo);
	for (int i = 0; i < native_list_count; i++)
	{
		//选中正在处理的项，给出一个处理提示
		info->pWorkList->SetCurSel(i);
		//置空数组
		ZeroMemory(szSrcFileName, U_MAX_PATH_SIZE);
		ZeroMemory(szDstFileName, U_MAX_PATH_SIZE);
		//获取列表框字符串并转换字符编码
		TCHAR str[U_MAX_PATH_SIZE] = { 0 };
		info->pWorkList->GetText(i,str);
		strcpy(szSrcFileName,T2A(str));
		//获得文件名下标从路径
		int fni = GetFileNameIndexFromPath(szSrcFileName, pathSpliter);
		//根据操作类型，进行拼接目标路径
		if (info->bisEncrypte)
		{
			sprintf(szDstFileName, "%s%c%s%s\0", info->szSavePath, pathSpliter, &szSrcFileName[fni], (info->bisNoSuffix ? "" : lockFileSuffix));
		}
		else
		{
			sprintf(szDstFileName, "%s%c%s\0", info->szSavePath, pathSpliter, &szSrcFileName[fni]);
			//获取目标路径后缀，判断是否是使用默认后缀的文件，如果是则去除默认后缀
			int sfi = GetSuffixIndexFromPath(szDstFileName, pathSpliter);
			char suffix[20] = { 0 };
			strcpy(suffix, &szDstFileName[sfi]);
			StringToLower(suffix);
			if (strcmp(suffix, lockFileSuffix) == 0)
			{
				szDstFileName[sfi] = 0;
			}
		}
		//写入处理信息处理的源和目标
		sprintf(pinfo.szSrcFileName, "%s\0", &szSrcFileName[fni]);
		sprintf(pinfo.szDstFileName, "%s\0", &szDstFileName[GetFileNameIndexFromPath(szDstFileName, pathSpliter)]);
		//判断是否是文件夹，进行相关处理
		int ret = true;
		if (GetFileAttributes(str)&FILE_ATTRIBUTE_DIRECTORY)
		{
			//构造新的处理新题给目录处理使用，由于其他参数都一样，就直接内存拷贝即可
			EncrypteInfomation nextDirInfo = { 0 };
			memcpy(&nextDirInfo, info, sizeof(EncrypteInfomation));
			strcpy(nextDirInfo.szSavePath, szDstFileName);//拷贝新的保存路径
			EncryptDirectoryProc(&nextDirInfo,&pinfo, szSrcFileName);//开始目录递归处理
			ret = true;//防止删除源文件
		}
		else
		{
			if (info->bisEncrypte)//根据操作类型进行文件加解密
				ret = info->locker->lock(szSrcFileName, szDstFileName, info->szPassword);
			else
				ret = info->locker->unlock(szSrcFileName, szDstFileName, info->szPassword);
		}
		
		//判断并设置处理信息
		if (ret == true)
		{
			pinfo.nSuccessCount++;
			pinfo.bisSuccess = true;
		}
		else
		{
			pinfo.nFailureCount++;
			pinfo.bisSuccess = false;
		}
		//判断是否需要删除源文件，在操作成功时允许删除
		if (info->bisDelSource && ret)
			DeleteFileA(szSrcFileName);
		//发送处理信息
		SendMessage(info->hWnd, UM_THREAD_PROCESS_INFO, (WPARAM)(int)(i*1.0 / pinfo.nProcessCount * 100), (LPARAM)&pinfo);
	}
	//发送结束处理信息
	SendMessage(info->hWnd, UM_THREAD_PROCESS_INFO, (WPARAM)100, (LPARAM)&pinfo);

	return 0;
}
/*自定义消息处理*/
LRESULT CFileEncryptorProDlg::EncryptMsgProc(WPARAM wParam, LPARAM lParam)
{
	int process = (int)wParam;
	m_pro_Process.SetPos(process);
	if (process == 0)
	{
		//开始处理时就禁用控件
		disableControls();
	}
	if (process==100)
	{
		//结束处理时就启用功能
		enableControls();
		delete m_pEncrypteInfo;
		m_pEncrypteInfo = NULL;
	}
	//转换并显示处理信息
	PProcessInfo pinfo = (PProcessInfo)lParam;
	USES_CONVERSION;
	m_str_process_info.Format(TEXT("状态：%s %d%% 成/败/总:%d/%d/%d %s 对象:%s --> %s"),
		(pinfo->bisSuccess?TEXT("成功"):TEXT("失败")),
		process,
		pinfo->nSuccessCount,
		pinfo->nFailureCount,
		pinfo->nProcessCount,
		(process==100?TEXT("【任务已完成！】"):TEXT("")),
		A2T(pinfo->szSrcFileName),
		A2T(pinfo->szDstFileName)
		);
	UpdateData(FALSE);
	return 0;
}

/*
开始处理
*/
void CFileEncryptorProDlg::StartWorkProcess()
{
	char szSavePath[U_MAX_PATH_SIZE] = { 0 };
	char szPassword[MAXBYTE*2] = { 0 };
	UpdateData(TRUE);
	//获取处理信息并进行判定
	m_str_SavePath = m_str_SavePath.Trim();
	m_str_Password = m_str_Password.Trim();
	if (m_str_SavePath.GetLength() == 0)
	{
		MessageBox(TEXT("错误！保存路径不能为空"));
		return;
	}

	if (m_str_Password.GetLength() == 0)
	{
		MessageBox(TEXT("错误！密码不能为空"));
		return;
	}

	if (m_lb_WorkList.GetCount() == 0)
	{
		MessageBox(TEXT("错误！请先添加待处理项"));
		return;
	}
	//转换字节编码
	USES_CONVERSION;
	strcpy(szSavePath, T2A(m_str_SavePath));
	//strcpy(szPassword, T2A(m_str_Password));
	WCHAR wstr[MAXBYTE] = { 0 };
	lstrcpy(wstr, m_str_Password);
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, szPassword, size, NULL, NULL);

	
	UpdateData(FALSE);
	//创建处理信息
	m_pEncrypteInfo = new EncrypteInfomation();
	m_pEncrypteInfo->locker = &m_locker;
	m_pEncrypteInfo->hWnd = this->GetSafeHwnd();
	strcpy(m_pEncrypteInfo->szSavePath, szSavePath);
	strcpy(m_pEncrypteInfo->szPassword, szPassword);
	m_pEncrypteInfo->pWorkList = &m_lb_WorkList;
	m_pEncrypteInfo->bisEncrypte = ((IsDlgButtonChecked(IDC_RADIO_ENCRYPTE) == 1) ? true : false);
	m_pEncrypteInfo->bisNoSuffix = ((IsDlgButtonChecked(IDC_CHECK_NOT_ADD_SUFFIX) == 1) ? true : false);
	m_pEncrypteInfo->bisDelSource = ((IsDlgButtonChecked(IDC_CHECK_DELETE_SOURCE_FILE) == 1) ? true : false);
	//开启线程，进行处理
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EncryptThreadProc, (LPVOID)m_pEncrypteInfo, 0, NULL);

}

/*
启用相关功能供给用户使用
*/
void CFileEncryptorProDlg::enableControls()
{
	m_isRunning = false;
	m_btn_StartWork.EnableWindow(TRUE);
	m_btn_ClearAllItem.EnableWindow(TRUE);
	m_btn_RemoveSelectItem.EnableWindow(TRUE);
	m_btn_OpenFile.EnableWindow(TRUE);
	m_edit_SavePath.EnableWindow(TRUE);
	m_edit_Password.EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_ENCRYPTE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_DECRYTE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_DELETE_SOURCE_FILE))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_NOT_ADD_SUFFIX))->EnableWindow(TRUE);
}

/*
禁用相关功能，以免影响线程处理
*/
void CFileEncryptorProDlg::disableControls()
{
	m_isRunning = true;
	m_btn_StartWork.EnableWindow(FALSE);
	m_btn_ClearAllItem.EnableWindow(FALSE);
	m_btn_RemoveSelectItem.EnableWindow(FALSE);
	m_btn_OpenFile.EnableWindow(FALSE);
	m_edit_SavePath.EnableWindow(FALSE);
	m_edit_Password.EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ENCRYPTE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_DECRYTE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_DELETE_SOURCE_FILE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_NOT_ADD_SUFFIX))->EnableWindow(FALSE);
}

