#ifndef __KXMLREADER_KCREATEORGANIZEWIN_H__
#define __KXMLREADER_KCREATEORGANIZEWIN_H__

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

class KCreateOrganizeWin : public QWidget
{
	Q_OBJECT

public:
	KCreateOrganizeWin(QWidget *parent);
	~KCreateOrganizeWin();
signals:
	void orgBackMainWinSig();
	void orgDuplicateNameSig(bool isExist);
	void createOrgSig(bool isSuccess, const QString& output);
	void initTimerSig();
	void endTimerSig();
private slots:
	void onHandleOrganizeDuplicateNameQueryBtn();
	void onHandleCreateOrg();  
private:
	KCreateOrganizeWin(const KCreateOrganizeWin& other) = default;
	void operator=(const KCreateOrganizeWin& other) = delete;
	KCreateOrganizeWin(const KCreateOrganizeWin&& other) = delete;
	KCreateOrganizeWin& operator=(KCreateOrganizeWin&& other) = default;

	void initWindow();

	QLineEdit* m_pOrgLine;
	QPlainTextEdit* m_pOrgDescribeEdit;
	QPushButton* m_pCreateOrganizeOkBtn;
};
#endif
