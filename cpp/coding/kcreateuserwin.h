#ifndef __KXMLREADER_KCREATEUSERWIN_H__
#define __KXMLREADER_KCREATEUSERWIN_H__

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>

class KCreateUserWin : public QWidget
{
	Q_OBJECT

public:
	KCreateUserWin(QWidget *parent);
	~KCreateUserWin();
signals:
	void userBackMainWinSig();
	void createUserSig(const QString& output);
	void initTimerSig();
	void endTimerSig();
private slots:
	void onHandleCreateUserBtn();
	void onHandleUserInsertBtn();
private:
	KCreateUserWin(const KCreateUserWin& other) = default;
	void operator=(const KCreateUserWin& other) = delete;
	KCreateUserWin(const KCreateUserWin&& other) = delete;
	KCreateUserWin& operator=(KCreateUserWin&& other) = default;

	void initWindow();
	void initModel();

	void insertTableView();

	QPushButton* m_pCreateUserOkBtn;

	QTableView* m_pTableView;
	QStandardItemModel* m_pModel;
};
#endif
