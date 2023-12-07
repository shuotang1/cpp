#include "kfilewin.h"
#include "kglobaldata.h"
#include "krallfile.h"
#include "kgitprocess.h"
#include "kdatabase.h"

#include <QSplitter>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QDesktopServices>

KFileWin::KFileWin(QWidget *parent)
	: QWidget(parent)
	, m_pRootDirLine(nullptr)
	, m_pSubDirLine(nullptr)
	, m_pDirModel(nullptr)
	, m_pTreeView(nullptr)
	, m_pWeb(nullptr)
	, m_pNextBtn(nullptr)
	, m_pCurrentDirLabel(nullptr)
	, m_pPreBtn(nullptr)
	, m_pScoringWin(nullptr)
	, m_pWebBackgroundBox(nullptr)
	, m_pCompileBtn(nullptr)
	, m_pSearchBtn(nullptr)
	, m_pSearchWin(nullptr)
	, m_pWebFontSize(nullptr)
	, m_pOpenDirTreeFileMenu(nullptr)
	, m_index(0)
{
	initWindow();
	m_file.reserve(150);
	m_dirPath.reserve(150);
}

KFileWin::~KFileWin()
{
}

void KFileWin::initWindow()
{
	QLabel* rootLabel = new QLabel(tr("rootLabelTr") + ":", this);
	m_pRootDirLine = new QLineEdit(this);
	QPushButton* rootDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), this);
	QLabel* subLabel = new QLabel(tr("subLabelTr") + ":", this);
	m_pSubDirLine = new QLineEdit(this);
	m_pSubDirLine->setReadOnly(true);
	QPushButton* subDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), this);
	m_pWebBackgroundBox = new QCheckBox(tr("webStyleBox"), this);
	m_pSearchBtn = new QPushButton(QIcon(":/icons/search.png"), QStringLiteral(""), this);
	m_pSearchBtn->setEnabled(false);
	m_pSearchBtn->setShortcut(Qt::CTRL + Qt::Key_F);        //查询快捷键:ctrl+f
	QLabel* webFontSizeLabel = new QLabel(tr("webFontSizeLabelTr") + ":", this);
	m_pWebFontSize = new QSpinBox(this);
	m_pWebFontSize->setValue(10);   //web文字默认10pt

	QHBoxLayout* m_pHLineLay = new QHBoxLayout;
	m_pHLineLay->addWidget(rootLabel);
	m_pHLineLay->addWidget(m_pRootDirLine);
	m_pHLineLay->addWidget(rootDirBtn);
	m_pHLineLay->addWidget(subLabel);
	m_pHLineLay->addWidget(m_pSubDirLine);
	m_pHLineLay->addWidget(subDirBtn);
	m_pHLineLay->addWidget(m_pWebBackgroundBox);
	m_pHLineLay->addWidget(webFontSizeLabel);
	m_pHLineLay->addWidget(m_pWebFontSize);
	m_pHLineLay->addWidget(m_pSearchBtn);

	QFont font;
	font.setPointSize(12);
	m_pCurrentDirLabel = new QLabel(tr("currentDirLabelTr") + ":", this);
	m_pCurrentDirLabel->setFont(font);
	QSplitter* splitter = new QSplitter(this);
	m_pDirModel = new QDirModel(this);
	m_pTreeView = new QTreeView(splitter);
	m_pTreeView->setModel(m_pDirModel);
	m_pTreeView->setColumnHidden(1, true);       //省略大小
	m_pTreeView->setColumnHidden(2, true);       //省略类型
	m_pTreeView->setColumnHidden(3, true);       //省略最后修改时间
	m_pNextBtn = new KNextBtn(tr("nextBtnTr"), this);
	m_pNextBtn->setEnabled(false);
	m_pNextBtn->setFont(font);
	m_pPreBtn = new KNextBtn(tr("preBtnTr"), this);
	m_pPreBtn->setEnabled(false);
	m_pPreBtn->setFont(font);
	m_pCompileBtn = new QPushButton(tr("compileBtn"), this);
	m_pCompileBtn->setEnabled(false);
	m_pCompileBtn->setFont(font);

	m_pOpenDirTreeFileMenu = new QMenu(m_pTreeView);
	QAction* openLocationAction = new QAction(tr("openLocationActionTr"), m_pOpenDirTreeFileMenu);
	m_pOpenDirTreeFileMenu->addAction(openLocationAction);
	m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

	QHBoxLayout* m_pHBtnLay = new QHBoxLayout;
	m_pHBtnLay->addWidget(m_pCompileBtn);
	m_pHBtnLay->addWidget(m_pPreBtn);
	m_pHBtnLay->addWidget(m_pNextBtn);

	QDockWidget* dockWidget = new QDockWidget(tr("dockWidgetTr"), this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	dockWidget->setFeatures(QDockWidget::DockWidgetFloatable);
	dockWidget->setStyleSheet("QDockWidget::title { text-align: center;background-color: transparent; }");
	m_pScoringWin = new KScoringWin(dockWidget);
	dockWidget->setWidget(m_pScoringWin);

	QVBoxLayout* m_pVDirLay = new QVBoxLayout;
	m_pVDirLay->addWidget(m_pCurrentDirLabel);
	m_pVDirLay->addWidget(splitter, 10);
	m_pVDirLay->addLayout(m_pHBtnLay);
	m_pVDirLay->addWidget(dockWidget, 10);

	m_pWeb = new QWebEngineView(this);
	
	QHBoxLayout* m_pHLay = new QHBoxLayout;
	m_pHLay->addLayout(m_pVDirLay);
	m_pHLay->addWidget(m_pWeb, 2);

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->addLayout(m_pHLineLay);
	m_pVMainLay->addLayout(m_pHLay);

	(void)connect(rootDirBtn, &QPushButton::clicked, this, &KFileWin::handleRootDirClick);
	(void)connect(subDirBtn, &QPushButton::clicked, this, &KFileWin::handleSubDirClick);
	(void)connect(m_pNextBtn, &QPushButton::clicked, this, &KFileWin::handleNextClick);
	(void)connect(m_pPreBtn, &QPushButton::clicked, this, &KFileWin::handlePreClick);
	(void)connect(m_pNextBtn, &KNextBtn::sendMenuActionSignal, this, &KFileWin::handleNextBtnSingal);
	(void)connect(m_pPreBtn, &KNextBtn::sendMenuActionSignal, this, &KFileWin::handleNextBtnSingal);
	(void)connect(m_pTreeView, &QTreeView::doubleClicked, this, &KFileWin::handleDirTreeDoubleClick);
	(void)connect(m_pCompileBtn, &QPushButton::clicked, this, &KFileWin::handleCompileClick);
	(void)connect(m_pSearchBtn, &QPushButton::clicked, this, &KFileWin::handleSearchClick);
	(void)connect(m_pWebBackgroundBox, &QCheckBox::stateChanged, this, &KFileWin::handleWebBackgroundChange);
	(void)connect(m_pWebFontSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &KFileWin::handleWebFontSizeChange);
	(void)connect(m_pTreeView, &QTreeView::customContextMenuRequested, this, &KFileWin::bindDirTreeAndMenu);
	(void)connect(openLocationAction, &QAction::triggered, this, &KFileWin::handleOpenDirTreeFileAction);

	(void)connect(m_pRootDirLine, &QLineEdit::textChanged, this, [=]() {
		m_pSubDirLine->setText("");
		m_pTreeView->setRootIndex(m_pDirModel->index(QDir::rootPath()));
		m_pCurrentDirLabel->setText(tr("currentDirLabelTr") + ":");
	});
}

void KFileWin::updateScoringWin()
{
	KDataBase db;
	QVector<QString> output;
	if (db.selectScore(m_file[m_index], output))
		m_pScoringWin->updateScoreWin(output[0], output[1]);
	else
		m_pScoringWin->updateScoreWin("", "");
}

void KFileWin::handleRootDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pRootDirLine->setText(folderPath);
}

void KFileWin::handleSubDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), m_pRootDirLine->text());
	if (folderPath.isEmpty())
		return;
	QString subDirPath = folderPath.remove(0, m_pRootDirLine->text().length());
	int firstSlashIndex = subDirPath.indexOf('/');
	subDirPath.replace(firstSlashIndex + 1, subDirPath.indexOf('/', firstSlashIndex + 1) - firstSlashIndex - 1, "*");
	m_pSubDirLine->setText(subDirPath);
	showDirTree();
}

void KFileWin::showDirTree()
{
	m_file.clear();
	m_dirPath.clear();
	QDir rootDir(m_pRootDirLine->text());
	QStringList fileList = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (const QString& file : fileList)
	{
		m_file.push_back(file);
		m_dirPath.push_back(m_pRootDirLine->text() + QDir::separator() + file + m_pSubDirLine->text().mid(2));
	}
	if (!m_dirPath.empty())
	{
		QDir dir(m_dirPath[m_index]);
		if (!dir.exists())
		{
			QMessageBox::warning(Q_NULLPTR, "dirNotExist", m_dirPath[m_index] + tr("dirNotExistTr"), QMessageBox::Ok);
			return;
		}
		m_pTreeView->setRootIndex(m_pDirModel->index(m_dirPath[m_index]));
		m_pCompileBtn->setEnabled(true);
		m_pPreBtn->setEnabled(true);
		m_pNextBtn->setEnabled(true);
		m_pCurrentDirLabel->setText(m_file[m_index] + ":");
		m_pPreBtn->converDirToNextBtn(m_dirPath);
		m_pNextBtn->converDirToNextBtn(m_dirPath);
	}
}

void KFileWin::handleNextClick()
{
	m_pScoringWin->saveScoreToDb(m_file[m_index]);

	m_index++;
	if (m_index < m_dirPath.size())
	{
		QDir dir(m_dirPath[m_index]);
		if (!dir.exists())
		{
			QMessageBox::warning(Q_NULLPTR, "dirNotExist", m_dirPath[m_index] + tr("dirNotExistClickNextTr"), QMessageBox::Ok);
			return;
		}
		m_pCurrentDirLabel->setText(m_file[m_index] + ":");
		m_pTreeView->setRootIndex(m_pDirModel->index(m_dirPath[m_index]));
		updateScoringWin();
	}
	else
	{
		m_index--;
		QMessageBox::warning(Q_NULLPTR, "hasArrivedBottom", tr("hasArrivedBottomTr"), QMessageBox::Ok);
	}
}

void KFileWin::handlePreClick()
{
	m_pScoringWin->saveScoreToDb(m_file[m_index]);

	m_index--;
	if (m_index >= 0)
	{
		QDir dir(m_dirPath[m_index]);
		if (!dir.exists())
		{
			QMessageBox::warning(Q_NULLPTR, "dirNotExist", m_dirPath[m_index] + tr("dirNotExistClickPreTr"), QMessageBox::Ok);
			return;
		}
		m_pCurrentDirLabel->setText(m_file[m_index] + ":");
		m_pTreeView->setRootIndex(m_pDirModel->index(m_dirPath[m_index]));
		updateScoringWin();
	}
	else
	{
		m_index++;
		QMessageBox::warning(Q_NULLPTR, "hasArrivedTop", tr("hasArrivedTopTr"), QMessageBox::Ok);
	}
}

void KFileWin::handleNextBtnSingal(const QString& dir)
{
	m_pScoringWin->saveScoreToDb(m_file[m_index]);

	m_pTreeView->setRootIndex(m_pDirModel->index(dir));
	m_index = m_dirPath.indexOf(dir);
	m_pCurrentDirLabel->setText(m_file[m_index] + ":");
	updateScoringWin();
}

void KFileWin::showFileInWeb(const QString& filePath)
{
	KGitProcess git;
	QString fontSize = QString::number(m_pWebFontSize->value()) + "pt";
	if (m_pWebBackgroundBox->isChecked())
		git.inputHighLightFileArg(filePath, "black", fontSize);
	else
		git.inputHighLightFileArg(filePath, "default", fontSize);
	git.start();
	while (!git.isFinished())
	{
		QCoreApplication::processEvents();
	}
	m_pWeb->load(QUrl::fromLocalFile(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_htmlFileDir));
	m_pSearchBtn->setEnabled(true);
}

void KFileWin::handleDirTreeDoubleClick(const QModelIndex& index)
{
	QString filepath = m_pDirModel->fileInfo(index).absoluteFilePath();
	if (!m_pDirModel->isDir(index))
	{
		m_filePath = filepath;
		showFileInWeb(filepath);
	}
}

void KFileWin::handleWebBackgroundChange(bool isChecked)
{
	if(!m_filePath.isEmpty())
		showFileInWeb(m_filePath);
}

void KFileWin::handleWebFontSizeChange(int value)
{
	if (!m_filePath.isEmpty())
		showFileInWeb(m_filePath);
}

void KFileWin::handleCompileClick()
{
	emit compileSignal(m_dirPath[m_index]);
}

void KFileWin::handleSearchClick()
{
	m_pSearchWin = std::make_unique<KSearchWin>(m_pWeb);
	m_pSearchWin->setParent(m_pWeb);
	QPoint webTopRight = m_pWeb->geometry().topRight();
	m_pSearchWin->move(webTopRight.x() - m_pSearchWin->width(), webTopRight.y() - 40);  //窗口左上角移动
	m_pSearchWin->resize(300, 50);
	m_pSearchWin->show();
}

void KFileWin::bindDirTreeAndMenu(const QPoint& pos)
{
	QModelIndex index = m_pTreeView->indexAt(pos);
	if (index.isValid())
		m_pOpenDirTreeFileMenu->exec(m_pTreeView->viewport()->mapToGlobal(pos));     //显示右键菜单
}

void KFileWin::handleOpenDirTreeFileAction()
{
	QModelIndex index = m_pTreeView->currentIndex();
	if (index.isValid()) 
	{
		QString filePath = m_pDirModel->fileInfo(index).absoluteFilePath();
		QFileInfo fileInfo(filePath);
		QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
	}
}