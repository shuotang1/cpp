#include "kmainwindow.h"
#include "kglobaldata.h"
#include "ksimwin.h"
#include "kcompilewin.h"
#include "kfilewin.h"
#include "kcppcheckwin.h"

#include <QMenuBar>
#include <QDesktopServices>
#include <QEvent>
#include <QToolTip>
#include <QCoreApplication>
#include <QHBoxLayout>
#include <QUrl>

KMainWindow::KMainWindow(QWidget* parent)
	: QWidget(parent)
	, m_pStackedWin(nullptr)
	, m_pSaveWin(nullptr)
	, m_pTotalSimAction(nullptr)
	, m_pSpecSimAction(nullptr)
	, m_pCompareSimAction(nullptr)
	, m_pCheckSymbolAct(nullptr)
	, m_pCheckMsgAct(nullptr)
	, m_pCheckVerboseAct(nullptr)
	, m_pCheckIdAct(nullptr)
	, m_pCheckLineAct(nullptr)
	, m_pCheckColumnAct(nullptr)
	, m_pCheckInfoAct(nullptr)
	, m_pCheckSeverityAct(nullptr)
	, m_pCheckCweAct(nullptr)
	, m_pCompareDirSimAction(nullptr)
	, m_pToolWin(nullptr)
	, m_pToolBar(nullptr)
	, m_pSignalMapper(nullptr)
	, m_pTotalCheckAction(nullptr)
	, m_pSpecCheckAction(nullptr)
	, m_toolActionId(0)
	, m_lastToolActionSelectedId(0)
{
	init();
}

KMainWindow::~KMainWindow()
{
}

void KMainWindow::init()
{
	setMinimumSize(1200, 800);

	QMenuBar* menuBar = new QMenuBar(this);
	QMenu* fileMenu = new QMenu(tr("fileMenuTr"), menuBar);
	menuBar->addMenu(fileMenu);
	QAction* saveCSVAction = new QAction(QIcon(":/icons/csv.png"), QObject::tr("saveCSVAction"), this);
	fileMenu->addAction(saveCSVAction);
	QMenu* feedBackMenu = new QMenu(tr("feedBackMenu"), menuBar);
	menuBar->addMenu(feedBackMenu);
	QAction* feedBackAction = new QAction(QIcon(":/icons/feedback.png"), tr("feedBackMenu"), this);
	feedBackMenu->addAction(feedBackAction);

	QActionGroup* actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);
	QMenu* simMenu = new QMenu(tr("winChangeMenuTr"), menuBar);
	menuBar->addMenu(simMenu);
	simMenu->setToolTipsVisible(true);
	m_pTotalSimAction = new QAction(QIcon(":/icons/total.png"), tr("totalSimActionTr"), this);
	m_pTotalSimAction->setCheckable(true);
	m_pTotalSimAction->setChecked(true);
	m_pTotalSimAction->setToolTip(tr("totalSimActionToolTipTr"));
	actionGroup->addAction(m_pTotalSimAction);
	simMenu->addAction(m_pTotalSimAction);
	m_pSpecSimAction = new QAction(QIcon(":/icons/blackspec.png"),tr("specSimActionTr") , this);
	m_pSpecSimAction->setCheckable(true);
	m_pSpecSimAction->setToolTip(tr("specSimActionToolTipTr"));
	actionGroup->addAction(m_pSpecSimAction);
	simMenu->addAction(m_pSpecSimAction);
	m_pCompareSimAction = new QAction(QIcon(":/icons/blackvs.png"), tr("compareSimActionTr"), simMenu);
	m_pCompareSimAction->setCheckable(true);
	actionGroup->addAction(m_pCompareSimAction);
	simMenu->addAction(m_pCompareSimAction);
	m_pCompareDirSimAction = new QAction(QIcon(":/icons/blackdirvs.png"), tr("compareDirSimActionTr"), simMenu);
	m_pCompareDirSimAction->setCheckable(true);
	actionGroup->addAction(m_pCompareDirSimAction);
	simMenu->addAction(m_pCompareDirSimAction);
	m_pTotalCheckAction = new QAction(QIcon(":/icons/blackTotal.png"), tr("totalCheckActionTr"), this);
	m_pTotalCheckAction->setCheckable(true);
	actionGroup->addAction(m_pTotalCheckAction);
	simMenu->addAction(m_pTotalCheckAction);
	m_pSpecCheckAction = new QAction(QIcon(":/icons/blackspec.png"), tr("specCheckActionTr"), this);
	m_pSpecCheckAction->setCheckable(true);
	actionGroup->addAction(m_pSpecCheckAction);
	simMenu->addAction(m_pSpecCheckAction);

	QMenu* checkTableMenu = new QMenu(tr("checkTableMenuTr"), menuBar);
	menuBar->addMenu(checkTableMenu);
	QAction* checkFileAct = new QAction(tr("checkFileActTr"), checkTableMenu);
	checkFileAct->setCheckable(true);
	checkFileAct->setChecked(true);
	checkFileAct->setEnabled(false);  
	checkTableMenu->addAction(checkFileAct);
	m_pCheckSymbolAct = new QAction(tr("checkSymbolActTr"), checkTableMenu);
	m_pCheckSymbolAct->setCheckable(true);
	m_pCheckSymbolAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckSymbolAct);
	m_pCheckMsgAct = new QAction(tr("checkMsgActTr"), checkTableMenu);
	m_pCheckMsgAct->setCheckable(true);
	m_pCheckMsgAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckMsgAct);
	m_pCheckVerboseAct = new QAction(tr("checkVerboseActTr"), checkTableMenu);
	m_pCheckVerboseAct->setCheckable(true);
	m_pCheckVerboseAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckVerboseAct);
	m_pCheckIdAct = new QAction(tr("checkIdActTr"), checkTableMenu);
	m_pCheckIdAct->setCheckable(true);
	m_pCheckIdAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckIdAct);
	m_pCheckLineAct = new QAction(tr("checkLineActTr"), checkTableMenu);
	m_pCheckLineAct->setCheckable(true);
	m_pCheckLineAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckLineAct);
	m_pCheckColumnAct = new QAction(tr("checkColumnActTr"), checkTableMenu);
	m_pCheckColumnAct->setCheckable(true);
	m_pCheckColumnAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckColumnAct);
	m_pCheckInfoAct = new QAction(tr("checkInfoActTr"), checkTableMenu);
	m_pCheckInfoAct->setCheckable(true);
	m_pCheckInfoAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckInfoAct);
	m_pCheckSeverityAct = new QAction(tr("checkSeverityActTr"), checkTableMenu);
	m_pCheckSeverityAct->setCheckable(true);
	m_pCheckSeverityAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckSeverityAct);
	m_pCheckCweAct = new QAction(tr("checkCweActTr"), checkTableMenu);
	m_pCheckCweAct->setCheckable(true);
	m_pCheckCweAct->setChecked(true);
	checkTableMenu->addAction(m_pCheckCweAct);

	QMenu* toolMenu = new QMenu(tr("toolMenuTr"), menuBar);
	menuBar->addMenu(toolMenu);
	QAction* setOpenDirDefaultPathAction = new QAction(QIcon(":/icons/defaultDirPath.png"), tr("setOpenDirDefaultPathActionTr"), this);
	toolMenu->addAction(setOpenDirDefaultPathAction);

	m_pToolBar = new QToolBar();
	m_pToolBar->setOrientation(Qt::Vertical);
	m_pToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	m_pStackedWin = new QStackedWidget(this);
	
	//  最终布局
	QHBoxLayout* m_pHMainLay = new QHBoxLayout(this);
	m_pHMainLay->addWidget(m_pToolBar);
	m_pHMainLay->addWidget(m_pStackedWin);
	m_pHMainLay->setMenuBar(menuBar);

	(void)connect(saveCSVAction, &QAction::triggered, this, &KMainWindow::onHandleSaveCSV);

	(void)connect(m_pTotalSimAction, &QAction::triggered, this, &KMainWindow::onHandleTotalSimWinChange);
	(void)connect(m_pSpecSimAction, &QAction::triggered, this, &KMainWindow::onHandleSpecSimWinChange);
	(void)connect(m_pCompareSimAction, &QAction::triggered, this, &KMainWindow::onHandleCompareSimWinChange);
	(void)connect(m_pCompareDirSimAction, &QAction::triggered, this, &KMainWindow::onHandleCompareDirSimWinChange);
	(void)connect(m_pTotalCheckAction, &QAction::triggered, this, &KMainWindow::onHandleTotalCheckWinChange);
	(void)connect(m_pSpecCheckAction, &QAction::triggered, this, &KMainWindow::onHandleSpecCheckWinChange);

	(void)connect(feedBackAction, &QAction::triggered, this, [&]() {
		QUrl url("https://kdocs.cn/l/cgToMtWRe0GI");
		QDesktopServices::openUrl(url);
	});

	(void)connect(m_pCheckSymbolAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckMsgAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckVerboseAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckIdAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckLineAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckColumnAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckInfoAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckSeverityAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);
	(void)connect(m_pCheckCweAct, &QAction::toggled, this, &KMainWindow::onHandleCheckActToggled);

	(void)connect(setOpenDirDefaultPathAction, &QAction::triggered, this, [&]() {
		m_pToolWin = std::make_unique<KToolWin>("setOpenDirDefaultPath", tr("setOpenDirDefaultPathTr"));
		m_pToolWin->show();
	});

	m_pSignalMapper = new QSignalMapper(this);
	(void)connect(m_pSignalMapper, SIGNAL(mapped(int)), this, SLOT(onHandleAddNewWidget(int)));
}

void KMainWindow::onHandleSaveCSV()
{
	m_pSaveWin = std::make_unique<KSaveWindow>();
	m_pSaveWin->show();
}

void KMainWindow::onHandleTotalSimWinChange()
{
	m_pTotalSimAction->setIcon(QIcon(":/icons/total.png"));
	m_pSpecSimAction->setIcon(QIcon(":/icons/blackspec.png"));
	m_pCompareSimAction->setIcon(QIcon(":/icons/blackvs.png"));
	m_pCompareDirSimAction->setIcon(QIcon(":/icons/blackdirvs.png"));
	m_pTotalCheckAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecCheckAction->setIcon(QIcon(":/icons/blackspec.png"));
	onHandleAddNewWidget(1);
	static_cast<KSimWin*>(m_pStackedWin->widget(1))->getStackedWin()->setCurrentIndex(0);
}

void KMainWindow::onHandleSpecSimWinChange()
{
	m_pTotalSimAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecSimAction->setIcon(QIcon(":/icons/spec.png"));
	m_pCompareSimAction->setIcon(QIcon(":/icons/blackvs.png"));
	m_pCompareDirSimAction->setIcon(QIcon(":/icons/blackdirvs.png"));
	m_pTotalCheckAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecCheckAction->setIcon(QIcon(":/icons/blackspec.png"));
	onHandleAddNewWidget(1);
	static_cast<KSimWin*>(m_pStackedWin->widget(1))->getStackedWin()->setCurrentIndex(1);
}

void KMainWindow::onHandleCompareSimWinChange()
{
	m_pTotalSimAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecSimAction->setIcon(QIcon(":/icons/blackspec.png"));
	m_pCompareSimAction->setIcon(QIcon(":/icons/vs.png"));
	m_pCompareDirSimAction->setIcon(QIcon(":/icons/blackdirvs.png"));
	m_pTotalCheckAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecCheckAction->setIcon(QIcon(":/icons/blackspec.png"));
	onHandleAddNewWidget(1);
	static_cast<KSimWin*>(m_pStackedWin->widget(1))->getStackedWin()->setCurrentIndex(2);
}

void KMainWindow::onHandleCompareDirSimWinChange()
{
	m_pTotalSimAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecSimAction->setIcon(QIcon(":/icons/blackspec.png"));
	m_pCompareSimAction->setIcon(QIcon(":/icons/blackvs.png"));
	m_pCompareDirSimAction->setIcon(QIcon(":/icons/dirvs.png"));
	m_pTotalCheckAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecCheckAction->setIcon(QIcon(":/icons/blackspec.png"));
	onHandleAddNewWidget(1);
	static_cast<KSimWin*>(m_pStackedWin->widget(1))->getStackedWin()->setCurrentIndex(3);
}

void KMainWindow::onHandleTotalCheckWinChange()
{
	m_pTotalSimAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecSimAction->setIcon(QIcon(":/icons/blackspec.png"));
	m_pCompareSimAction->setIcon(QIcon(":/icons/blackvs.png"));
	m_pCompareDirSimAction->setIcon(QIcon(":/icons/blackdirvs.png"));
	m_pTotalCheckAction->setIcon(QIcon(":/icons/total.png"));
	m_pSpecCheckAction->setIcon(QIcon(":/icons/blackspec.png"));
	onHandleAddNewWidget(0);
	static_cast<KCppCheckWin*>(m_pStackedWin->widget(0))->getStackedWin()->setCurrentIndex(0);
}

void KMainWindow::onHandleSpecCheckWinChange()
{
	m_pTotalSimAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecSimAction->setIcon(QIcon(":/icons/blackspec.png"));
	m_pCompareSimAction->setIcon(QIcon(":/icons/blackvs.png"));
	m_pCompareDirSimAction->setIcon(QIcon(":/icons/blackdirvs.png"));
	m_pTotalCheckAction->setIcon(QIcon(":/icons/blackTotal.png"));
	m_pSpecCheckAction->setIcon(QIcon(":/icons/spec.png"));
	onHandleAddNewWidget(0);
	static_cast<KCppCheckWin*>(m_pStackedWin->widget(0))->getStackedWin()->setCurrentIndex(1);
}

void KMainWindow::onHandleCheckActToggled(bool checked)
{
	QString isSelected = "1";
	isSelected = isSelected + ((m_pCheckSymbolAct->isChecked()) ? "1" : "0") + ((m_pCheckMsgAct->isChecked()) ? "1" : "0") +
		((m_pCheckVerboseAct->isChecked()) ? "1" : "0") + ((m_pCheckIdAct->isChecked()) ? "1" : "0") +
		((m_pCheckLineAct->isChecked()) ? "1" : "0") + ((m_pCheckColumnAct->isChecked()) ? "1" : "0") +
		((m_pCheckInfoAct->isChecked()) ? "1" : "0") + ((m_pCheckSeverityAct->isChecked()) ? "1" : "0") +
		((m_pCheckCweAct->isChecked()) ? "1" : "0");
	KGlobalDataNameSpace::m_isSelected = isSelected;
}

void KMainWindow::addNewWidget(const QString& blackIconDir, const QString& lightIconDir, const QString& title, QWidget* widget)
{
	QAction* toolAction = new QAction(QIcon(blackIconDir), title, this);
	m_pToolBar->addAction(toolAction);
	
	m_blackIconDirVec.push_back(blackIconDir);
	m_lightIconDirVec.push_back(lightIconDir);

	m_pStackedWin->addWidget(widget);

	m_toolActionMap[m_toolActionId] = toolAction;
	m_pSignalMapper->setMapping(toolAction, m_toolActionId);
	m_toolActionId++;

	(void)connect(toolAction, SIGNAL(triggered()), m_pSignalMapper, SLOT(map()));
	if(m_toolActionId == 5)            //查看代码窗口发出编译信号，处理的信号与槽函数
		(void)connect(static_cast<KFileWin*>(m_pStackedWin->widget(4)), &KFileWin::compileSignal, this, &KMainWindow::onHandleCompileSignal);
}

void KMainWindow::onHandleAddNewWidget(int actionId)
{
	m_toolActionMap[m_lastToolActionSelectedId]->setIcon(QIcon(m_blackIconDirVec[m_lastToolActionSelectedId]));
	m_toolActionMap[actionId]->setIcon(QIcon(m_lightIconDirVec[actionId]));
	m_pStackedWin->setCurrentIndex(actionId);
	m_lastToolActionSelectedId = actionId;
	QCoreApplication::processEvents();
}

void KMainWindow::setDefaultSelected(int index)
{
	m_toolActionMap[index]->setIcon(QIcon(m_lightIconDirVec[index]));
}

void KMainWindow::onHandleCompileSignal(const QString& dirPath)
{
	onHandleAddNewWidget(2);
	static_cast<KCompileWin*>(m_pStackedWin->widget(2))->startCompile(dirPath);
}