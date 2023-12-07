#ifndef __KXMLREADER_KCREATESTOREWIN_H__
#define __KXMLREADER_KCREATESTOREWIN_H__

#include "ktoolwin.h"

#include <QWidget>
#include <QCheckBox>

class KCreateStoreWin : public QWidget
{
	Q_OBJECT

public:
	KCreateStoreWin(QWidget *parent);
	~KCreateStoreWin();
signals:
	void repoBackMainWinSig();
	void initTimerSig();
	void endTimerSig();
	void configPersonDefaultFilePathEndSignal();
	void createRepoSig(const QString& output);
	void bindUserAndRepoSig(const QString& output);
	void personConfigDefaultSig(const QString& dir);
private slots:
	void onHandleCreateRepoBtn();
	void onHandleRepoInsertBtn();
	void onHandleBindUserAndRepoBtn();
	void onHandlePersonDefaultBtn();
private:
	KCreateStoreWin(const KCreateStoreWin& other) = default;
	void operator=(const KCreateStoreWin& other) = delete;
	KCreateStoreWin(const KCreateStoreWin&& other) = delete;
	KCreateStoreWin& operator=(KCreateStoreWin&& other) = default;

	void initWindow();
	void clearTempFolder();        // 清除copy后临时Temp文件夹

	QLineEdit* m_pLineEdit;
	QCheckBox* m_pIsPrivateRepo;
	QCheckBox* m_pIsAddDefaultFile;
	QLineEdit* m_pPersonDefaultDirLine;

	std::unique_ptr<KToolWin> m_pToolWin;
};
#endif
