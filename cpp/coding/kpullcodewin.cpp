// -------------------------------
// kpullcodewin.cpp
// 创建时间:2023/20/27
// 功能描述:从远方仓库拉取代码窗口
// --------------------------------

#include "kpullcodewin.h"
#include "kglobaldata.h"
#include "kgitprocess.h"

#include <QLabel>
#include <QIcon>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QProcess>
#include <QFormLayout>
#include <QCoreApplication>

KPullCodeWin::KPullCodeWin(QWidget *parent)
	: QWidget(parent)
	, m_pOrgLineEdit(nullptr)
	, m_pOutputLineEdit(nullptr)
	, m_pPullBackMainWinBtn(nullptr)
{
	initWindow();
}

KPullCodeWin::~KPullCodeWin()
{
}

void KPullCodeWin::initWindow()
{
	QLabel* orgNameLabel = new QLabel(tr("orgNameLabelTr") + ":", this);
	QLabel* outputLabel = new QLabel(tr("outputLabelTr") + ":", this);
	QFont font;
	font.setPointSize(12);
	orgNameLabel->setFont(font);
	outputLabel->setFont(font);
	m_pOrgLineEdit = new QLineEdit(this);
	m_pOutputLineEdit = new QLineEdit(this);
	QPushButton* pullBtn = new QPushButton(QIcon(":/icons/runoutput.png"), tr("pullBtnTr"), this);
	pullBtn->setEnabled(false);
	QPushButton* dirBtn = new QPushButton(QIcon(":/icons/folder.png"), "", this);
	m_pPullBackMainWinBtn = new QPushButton(QIcon(":/icons/main.png"), tr("backMainWinTr"), this);

	QHBoxLayout* m_pHDirLay = new QHBoxLayout;
	m_pHDirLay->addWidget(m_pOutputLineEdit);
	m_pHDirLay->addWidget(dirBtn);

	QFormLayout* headerLayout = new QFormLayout;
	headerLayout->setSpacing(10);
	headerLayout->addRow(orgNameLabel, m_pOrgLineEdit);
	headerLayout->addRow(outputLabel, m_pHDirLay);
	QHBoxLayout* m_pHBtnLay = new QHBoxLayout;
	pullBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pPullBackMainWinBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pHBtnLay->addWidget(pullBtn);
	m_pHBtnLay->addWidget(m_pPullBackMainWinBtn);
	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->addLayout(headerLayout);
	m_pVMainLay->addLayout(m_pHBtnLay);

	(void)connect(dirBtn, &QPushButton::clicked, this, &KPullCodeWin::onHandleDirClick);
	(void)connect(pullBtn, &QPushButton::clicked, this, &KPullCodeWin::onHandleRunClick);

	(void)connect(m_pPullBackMainWinBtn, &QPushButton::clicked, this, [&]() {
		emit pullBackMainWinClick();
	});

	(void)connect(m_pOrgLineEdit, &QLineEdit::textChanged, this, [=]() {
		pullBtn->setEnabled(!(m_pOrgLineEdit->text().isEmpty() || m_pOutputLineEdit->text().isEmpty()));
	});
	(void)connect(m_pOutputLineEdit, &QLineEdit::textChanged, this, [=]() {
		pullBtn->setEnabled(!(m_pOrgLineEdit->text().isEmpty() || m_pOutputLineEdit->text().isEmpty()));
	});
}

void KPullCodeWin::onHandleDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pOutputLineEdit->setText(folderPath);
}

void KPullCodeWin::onHandleRunClick()
{
	initTimerSig();
	KGitProcess gitProcess;
	gitProcess.pullCode(m_pOrgLineEdit->text(), m_pOutputLineEdit->text());
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	QString output = gitProcess.getOutput();
	pullCodeSig(output, m_pOutputLineEdit->text());
}