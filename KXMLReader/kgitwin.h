#ifndef __KXMLREADER_KGITWIN_H__
#define __KXMLREADER_KGITWIN_H__

#include <QWidget>
#include <QTextBrowser>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QTimer>

class KGitWin : public QWidget
{
	Q_OBJECT

public:
	KGitWin(QWidget* parent = nullptr);
	~KGitWin();
private slots:
	void onHandleChangeWinBtn(int index);

	void onHandleCreateOrg(bool isSuccess, const QString& output);
	void onHandleCreateUser(const QString& output);
	void onHandleCreateRepo(const QString& output);
	void onHandlePullCode(const QString& output, const QString& dir);
	void onHandleDelRepo(const QString& output);
	void onHandleDelUser(const QString& output);

	void onHandleOrganizeDuplicateNameQuery(bool isExist);
	void onHandleBindUserAndRepo(const QString& output);

	void onHandleConfigBtn();
	void personConfigDefault();

	void initTimerStart();
	void onHandleTimeout();
	void initTimerEnd();
private:
	KGitWin(const KGitWin& other) = default;
	void operator=(const KGitWin& other) = delete;
	KGitWin(const KGitWin&& other) = delete;
	KGitWin& operator=(KGitWin&& other) = default;

	void initWindow();
	void initConfig();

	QStackedWidget* m_pGitStackWin;
	QTextBrowser* m_pTextEdit;
	QProgressBar* m_pProgressBar;
	QTimer* m_pTimer;

	QLineEdit* m_pRootUrlEdit;
	QLineEdit* m_pAccessTokenEdit;

	QPushButton* m_pOrgWinChangeBtn;
	QPushButton* m_pUserWinChangeBtn;
	QPushButton* m_pRepoWinChangeBtn;
	QPushButton* m_pPullWinChangeBtn;
	QPushButton* m_pDelWinChangeBtn;
};
#endif
