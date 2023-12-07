#include "kcompilewin.h"
#include "kglobaldata.h"
#include "kcompile.h"

#include <QLabel>
#include <QPushButton>
#include <QCoreApplication>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>

KCompileWin::KCompileWin(QWidget *parent)
	: QWidget(parent)
	, m_pComDirLine(nullptr)
	, m_pTimer(nullptr)
	, m_pProgressBar(nullptr)
	, m_pCompileListWin(nullptr)
	, m_pExeWin(nullptr)
	, m_pPlainTextEdit(nullptr)
{
	initWin();
}

KCompileWin::~KCompileWin()
{
}

void KCompileWin::initWin()
{
	QLabel* compileLabel = new QLabel(tr("compileLabelTr") + ":", this);
	compileLabel->setToolTip(tr("compileLabelToolTipTr"));
	m_pComDirLine = new QLineEdit(this);
	QPushButton* comDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), this);
	QPushButton* runBtn = new QPushButton(tr("runBtnTr"), this);
	runBtn->setEnabled(false);

	QPushButton* openExeDirBtn = new QPushButton(QIcon(":/icons/open_folder.png"), tr("openExeDirBtnTr"), this);
	m_pCompileListWin = new QListWidget(this);
	m_pPlainTextEdit = new QPlainTextEdit(this);
	m_pPlainTextEdit->setReadOnly(true);

	QHBoxLayout* m_pHComLay = new QHBoxLayout;
	m_pHComLay->addWidget(compileLabel);
	m_pHComLay->addWidget(m_pComDirLine);
	m_pHComLay->addWidget(comDirBtn);
	m_pHComLay->addWidget(runBtn);

	QLabel* progressLabel = new QLabel(tr("progressLabelTr") + ": ", this);
	m_pProgressBar = new QProgressBar(this);
	QHBoxLayout* m_pHProgressLay = new QHBoxLayout;
	m_pHProgressLay->addWidget(progressLabel);
	m_pHProgressLay->addWidget(m_pProgressBar);

	QVBoxLayout* m_pVCompileMainLay = new QVBoxLayout(this);
	m_pVCompileMainLay->addLayout(m_pHComLay);
	m_pVCompileMainLay->addWidget(openExeDirBtn);
	m_pVCompileMainLay->addWidget(m_pCompileListWin);
	m_pVCompileMainLay->addWidget(m_pPlainTextEdit);
	m_pVCompileMainLay->addLayout(m_pHProgressLay);

	m_pTimer = new QTimer(this);

	(void)connect(comDirBtn, &QPushButton::clicked, this, &KCompileWin::handleComDirClick);
	(void)connect(m_pTimer, &QTimer::timeout, this, &KCompileWin::onHandleTimeout);
	(void)connect(runBtn, &QPushButton::clicked, this, &KCompileWin::onHandleCompileClick);
	(void)connect(openExeDirBtn, &QPushButton::clicked, this, &KCompileWin::onHandleOpenExeDirClick);

	(void)connect(m_pComDirLine, &QLineEdit::textChanged, this, [=]() {
		runBtn->setEnabled(!m_pComDirLine->text().isEmpty());
	});

	(void)connect(m_pCompileListWin, &QListWidget::itemDoubleClicked, [&](QListWidgetItem* item) {
		runExe(item->text());
	});
}


void KCompileWin::initTimerStart()
{
	m_pProgressBar->setMinimum(0);
	m_pProgressBar->setMaximum(0);
	m_pProgressBar->setValue(0);
	m_pTimer->start(100);
	QCoreApplication::processEvents();
}

void KCompileWin::onHandleTimeout()
{
	int value = m_pProgressBar->value();
	m_pProgressBar->setValue(value + 1);
	QCoreApplication::processEvents();
}

void KCompileWin::initTimerEnd()
{
	m_pTimer->stop();
	m_pProgressBar->setMaximum(100);
	m_pProgressBar->setValue(100);
}

void KCompileWin::handleComDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pComDirLine->setText(folderPath);
}

void KCompileWin::onHandleCompileClick()
{
	initTimerStart();
	m_pCompileListWin->clear();

	KGlobalDataNameSpace::m_compileDir = m_pComDirLine->text();

	KCompile compile(m_pComDirLine->text());
	compile.start();

	while (!compile.isFinished())
	{
		QCoreApplication::processEvents();
	}
	initTimerEnd();

	m_pPlainTextEdit->clear();
	QStringList compileLogs = compile.getDirLogs(m_pComDirLine->text());
	for (const QString& log : compileLogs)
	{
		m_pPlainTextEdit->appendPlainText(log);
	}

	QVector<QString> fileNames;
	fileNames.reserve(20);      //预分配存储编译结果exe名称容器大小为20
	compile.getExeFileName(fileNames);

	for (const QString& fileName : fileNames)
	{
		QListWidgetItem* item = new QListWidgetItem(QIcon(":/icons/exe.png"), fileName);
		m_pCompileListWin->addItem(item);
	}
}

void KCompileWin::runExe(const QString& fileName)
{
	m_pExeWin = std::make_unique<KExeWin>(fileName);
	m_pExeWin->show();
}

void KCompileWin::onHandleOpenExeDirClick()
{
	QString dir = KGlobalDataNameSpace::m_compileDir + "//exec";
	QUrl url = QUrl::fromLocalFile(dir);
	QDesktopServices::openUrl(dir);
}

void KCompileWin::startCompile(const QString& dirPath)
{
	m_pComDirLine->setText(dirPath);
	onHandleCompileClick();
}