#ifndef __KXMLREADER_KDELETEWIN_H__
#define __KXMLREADER_KDELETEWIN_H__

#include <QWidget>
#include <QLineEdit>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>

class KDeleteWin : public QWidget
{
	Q_OBJECT

public:
	KDeleteWin(QWidget *parent);
	~KDeleteWin();
signals:
	void delBackMainWinSig();
	void initTimerSig();
	void endTimerSig();
	void delRepoSig(const QString& output);
	void delUserSig(const QString& output);
private slots:
	void onHandleListRepoUser();
	void onHandleDelRepoBtn();
	void onHandleDelUserBtn();
	void showDelRepoDialog();
	void showDelUserDialog();
private:
	KDeleteWin(const KDeleteWin& other) = default;
	void operator=(const KDeleteWin& other) = delete;
	KDeleteWin(const KDeleteWin&& other) = delete;
	KDeleteWin& operator=(KDeleteWin&& other) = default;

	void initWindow();
	void initModel();
	void insertTableView();

	QLineEdit* m_pLineEdit;
	QTableView* m_pTableView;
	QStandardItemModel* m_pModel;
	QPushButton* m_pDelOrgRepoBtn;
	QPushButton* m_pDelOrgUserBtn;
};
#endif
