
// FileEncryptorProDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"
#include"EveryFileLocker.hpp"

#define UM_THREAD_PROCESS_INFO (WM_USER+0x25)//自定义消息，用来发送线程消息和界面交互的
#define U_MAX_PATH_SIZE 2048//自定义的最大路径长度
typedef struct _EncrypteInfomation//加解密信息报
{
	EveryFileLocker * locker;//加密对象
	HWND hWnd;//窗口句柄，接受自定义消息的窗口句柄
	CListBox * pWorkList;//获取列表项目的列表地址
	char szSavePath[U_MAX_PATH_SIZE];//当前的文件保存目录
	char szPassword[MAXBYTE];//密码
	bool bisEncrypte;//是否是加密，反之就是解密
	bool bisNoSuffix;//是否不使用默认后缀
	bool bisDelSource;//是否删除源文件
}EncrypteInfomation, *PEncrypteInfomation;
typedef struct _ProcessInfo//当前处理结果数据报
{
	char szSrcFileName[U_MAX_PATH_SIZE];//当前处理的文件
	char szDstFileName[U_MAX_PATH_SIZE];//当前保存的文件
	int nProcessCount;//所有处理项数
	int nSuccessCount;//成功项数
	int nFailureCount;//失败项数
	bool bisSuccess;//当前的处理是否成功
}ProcessInfo, *PProcessInfo;
// CFileEncryptorProDlg 对话框
class CFileEncryptorProDlg : public CDialogEx
{
public:
	EveryFileLocker m_locker;
// 构造
public:
	CFileEncryptorProDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FILEENCRYPTORPRO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	PEncrypteInfomation m_pEncrypteInfo;//加密解密信息报
	bool m_isRunning;//是否在进行加解密操作
	HANDLE m_hThread;//处理线程句柄
	CString m_str_process_info;//提示信息
	CString m_str_Password;//密码
	CString m_str_SavePath;//保存路径
	CString m_str_OpenFile;//打开的文件
	CListBox m_lb_WorkList;//列表框
	CButton m_btn_StartWork;//开始按钮
	CButton m_btn_ClearAllItem;//清空按钮
	CButton m_btn_RemoveSelectItem;//移除按钮
	CButton m_btn_OpenFile;//打开按钮
	CButton m_ckbtn_AddDefSuffix;//添加默认后缀选框
	CButton m_ckbtn_DeleteSourceFile;//删除源文件选框
	CMFCEditBrowseCtrl m_edit_OpenFile;//浏览文件框
	CMFCEditBrowseCtrl m_edit_SavePath;//浏览目录框
	CEdit m_edit_Password;//密码框
	CProgressCtrl m_pro_Process;//进度条框
	void InitControls();//初始化空间函数
	void RemoveSelectListItem();//移除选中函数
	void AddOpenFileToList();//添加项目到列表框函数
	void enableControls();//启用控件函数
	void disableControls();//禁用控件函数
	void StartWorkProcess();//开始处理函数
	void AddOnePathToList(TCHAR * path);//添加一个路径到列表框函数
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonOpenFile();
	afx_msg void OnBnClickedButtonOpenSavePath();
	afx_msg void OnBnClickedButtonRemoveSelectItem();
	afx_msg void OnBnClickedButtonClearAllItem();
	afx_msg void OnBnClickedButtonStartWork();
	afx_msg void OnDropFiles(HDROP hDropInfo);//文件拖入响应函数
	afx_msg LRESULT EncryptMsgProc(WPARAM wParam, LPARAM lParam);//自定义处理线程消息处理函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);//预处理消息重写
};
