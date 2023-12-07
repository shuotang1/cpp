#ifndef __KXMLREADER_KMAINWINDOW_H__
#define __KXMLREADER_KMAINWINDOW_H__

#include"ksavewindow.h"
#include"ktoolwin.h"

#include <QWidget>
#include <QStackedWidget>
#include <QToolBar>
#include <QMap>
#include <QSignalMapper>

class KMainWindow : public QWidget
{
	Q_OBJECT

public:
	KMainWindow(QWidget* parent = nullptr);
	~KMainWindow();

	void addNewWidget(const QString& blackIconDir, const QString& lightIconDir, const QString& title, QWidget* widget);
	void setDefaultSelected(int index);
private slots:
	void onHandleTotalSimWinChange();
	void onHandleSpecSimWinChange();
	void onHandleCompareSimWinChange();
	void onHandleCompareDirSimWinChange();
	void onHandleTotalCheckWinChange();
	void onHandleSpecCheckWinChange();

	void onHandleSaveCSV();

	void onHandleCheckActToggled(bool checked);
	void onHandleAddNewWidget(int actionId);
	void onHandleCompileSignal(const QString& dirPath);
private:
	KMainWindow(const KMainWindow& other) = delete;
	void operator=(const KMainWindow& other) = delete;
	KMainWindow(const KMainWindow&& other) = delete;
	KMainWindow& operator=(KMainWindow&& other) = delete;

	void init();

	QStackedWidget* m_pStackedWin;

	QAction* m_pTotalSimAction;
	QAction* m_pSpecSimAction;
	QAction* m_pCompareSimAction;
	QAction* m_pCompareDirSimAction;
	QAction* m_pTotalCheckAction;
	QAction* m_pSpecCheckAction;

	std::unique_ptr<KSaveWindow> m_pSaveWin;
	std::unique_ptr<KToolWin> m_pToolWin;

	QAction* m_pCheckSymbolAct;
	QAction* m_pCheckMsgAct;
	QAction* m_pCheckVerboseAct;
	QAction* m_pCheckIdAct;
	QAction* m_pCheckLineAct;
	QAction* m_pCheckColumnAct;
	QAction* m_pCheckInfoAct;
	QAction* m_pCheckSeverityAct;
	QAction* m_pCheckCweAct;

	QToolBar* m_pToolBar;
	int m_toolActionId;
	int m_lastToolActionSelectedId;
	QMap<int, QAction*> m_toolActionMap;
	QVector<QString> m_blackIconDirVec;
	QVector<QString> m_lightIconDirVec;
	QSignalMapper* m_pSignalMapper;
};

#endif