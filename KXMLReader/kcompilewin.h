#ifndef __KXMLREADER_KCOMPILEWIN_H__
#define __KXMLREADER_KCOMPILEWIN_H__

#include"kexewin.h"

#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QTimer>
#include <QProgressBar>
#include <QPlainTextEdit>

class KCompileWin : public QWidget
{
	Q_OBJECT

public:
	KCompileWin(QWidget* parent = nullptr);
	~KCompileWin();

	void startCompile(const QString& dirPath);
private slots:
	void handleComDirClick();
	void onHandleCompileClick();
	void onHandleTimeout();
	void onHandleOpenExeDirClick();
private:
	KCompileWin(const KCompileWin& other) = default;
	void operator=(const KCompileWin& other) = delete;
	KCompileWin(const KCompileWin&& other) = delete;
	KCompileWin& operator=(KCompileWin&& other) = default;

	void initWin();

	void initTimerStart();     //定时器启动，进度条开始滑动
	void initTimerEnd();       //定时器结束，进度条到达100

	void runExe(const QString& fileName);             //运行exe文件

	QLineEdit* m_pComDirLine;

	QListWidget* m_pCompileListWin;       //编译界面:QListWidget

	QTimer* m_pTimer;
	QProgressBar* m_pProgressBar;

	QPlainTextEdit* m_pPlainTextEdit;      //编译界面下显示错误类型的文本框

	std::unique_ptr<KExeWin> m_pExeWin;
};
#endif
