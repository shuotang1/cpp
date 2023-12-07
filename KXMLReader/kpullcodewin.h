#ifndef __KXMLREADER_KPULLCODEWIN_H__
#define __KXMLREADER_KPULLCODEWIN_H__

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class KPullCodeWin : public QWidget
{
	Q_OBJECT

public:
	KPullCodeWin(QWidget* parent = nullptr);
	~KPullCodeWin();
signals:
	void initTimerSig();
	void endTimerSig();
	void pullBackMainWinClick();
	void pullCodeSig(const QString& output, const QString& dir);
private slots:
	void onHandleDirClick();
	void onHandleRunClick();
private:
	KPullCodeWin(const KPullCodeWin& other) = default;
	void operator=(const KPullCodeWin& other) = delete;
	KPullCodeWin(const KPullCodeWin&& other) = delete;
	KPullCodeWin& operator=(KPullCodeWin&& other) = default;

	void initWindow();

	QLineEdit* m_pOrgLineEdit;
	QLineEdit* m_pOutputLineEdit;

	QPushButton* m_pPullBackMainWinBtn;
};
#endif
