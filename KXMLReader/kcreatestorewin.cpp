#include "kcreatestorewin.h"
#include "kgitprocess.h"
#include "kglobaldata.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTextStream>
#include <QCoreApplication>
#include <QLineEdit>

KCreateStoreWin::KCreateStoreWin(QWidget *parent)
	: QWidget(parent)
	, m_pLineEdit(nullptr)
	, m_pIsPrivateRepo(nullptr)
	, m_pIsAddDefaultFile(nullptr)
{
	initWindow();
}

KCreateStoreWin::~KCreateStoreWin()
{
}

void KCreateStoreWin::initWindow()
{
	QLabel* createStoreTitleLabel = new QLabel(tr("createStoreTitleLabelTr"), this);
	QFont storeFont = createStoreTitleLabel->font();
	storeFont.setPointSize(16);
	createStoreTitleLabel->setFont(storeFont);
	createStoreTitleLabel->setAlignment(Qt::AlignCenter);
	QFont font;
	font.setPointSize(12);
	QLabel* repoOrgName = new QLabel(tr("repoOrgNameTr") + ": ", this);
	repoOrgName->setFont(font);
	m_pLineEdit = new QLineEdit(this);
	m_pIsPrivateRepo = new QCheckBox(tr("privateRepoTr"), this);
	m_pIsPrivateRepo->setChecked(true);
	m_pIsPrivateRepo->setFont(font);
	m_pIsAddDefaultFile = new QCheckBox(tr("addDefaultTr"), this);
	m_pIsAddDefaultFile->setChecked(true);
	m_pIsAddDefaultFile->setFont(font);
	QPushButton* openAutoDefaultBtn = new QPushButton(tr("openAutoDefaultBtnTr"), this);
	QPushButton* configPersonDefaultDirBtn = new QPushButton(tr("configPersonDefaultDirBtnTr"), this);

	QPushButton* storeInsertBtn = new QPushButton(tr("repoInsertBtnTr"), this);
	QPushButton* createRepoOkBtn = new QPushButton(tr("createStoreOkBtnTr"), this);
	QPushButton* bindUserAndRepoBtn = new QPushButton(tr("bindUserAndRepoBtnTr"), this);
	QPushButton* repoBackMainWinBtn = new QPushButton(QIcon(":/icons/main.png"), tr("backMainWinTr"), this);
	createRepoOkBtn->setEnabled(false);
	bindUserAndRepoBtn->setEnabled(false);

	QHBoxLayout* m_pHCheckLay = new QHBoxLayout;
	m_pHCheckLay->addWidget(m_pIsPrivateRepo);
	m_pHCheckLay->addWidget(m_pIsAddDefaultFile);
	m_pHCheckLay->addWidget(openAutoDefaultBtn);
	m_pHCheckLay->addWidget(configPersonDefaultDirBtn);

	QHBoxLayout* m_pHRepoOrgLay = new QHBoxLayout;
	m_pHRepoOrgLay->addWidget(repoOrgName);
	m_pHRepoOrgLay->addWidget(m_pLineEdit);

	QHBoxLayout* createStoreLineLay = new QHBoxLayout;
	storeInsertBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	createRepoOkBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	repoBackMainWinBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	bindUserAndRepoBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	createStoreLineLay->addWidget(storeInsertBtn);
	createStoreLineLay->addWidget(createRepoOkBtn);
	createStoreLineLay->addWidget(bindUserAndRepoBtn);
	createStoreLineLay->addWidget(repoBackMainWinBtn);

	QVBoxLayout* m_pVStoreMainLay = new QVBoxLayout(this);
	m_pVStoreMainLay->setAlignment(Qt::AlignTop);
	m_pVStoreMainLay->addWidget(createStoreTitleLabel);
	m_pVStoreMainLay->addLayout(m_pHRepoOrgLay);
	m_pVStoreMainLay->addLayout(m_pHCheckLay);
	m_pVStoreMainLay->addLayout(createStoreLineLay);

	(void)connect(createRepoOkBtn, &QPushButton::clicked, this, &KCreateStoreWin::onHandleCreateRepoBtn);
	(void)connect(bindUserAndRepoBtn, &QPushButton::clicked, this, &KCreateStoreWin::onHandleBindUserAndRepoBtn);
	(void)connect(storeInsertBtn, &QPushButton::clicked, this, &KCreateStoreWin::onHandleRepoInsertBtn);

	(void)connect(repoBackMainWinBtn, &QPushButton::clicked, this, [&]() {
		emit repoBackMainWinSig();
	});
	(void)connect(m_pLineEdit, &QLineEdit::textChanged, this,[=]() {
		createRepoOkBtn->setEnabled(!m_pLineEdit->text().isEmpty());
		bindUserAndRepoBtn->setEnabled(!m_pLineEdit->text().isEmpty());
	});

	(void)connect(m_pIsAddDefaultFile, &QCheckBox::stateChanged, this, [=](int state) {
		if (state == Qt::Checked)
		{
			openAutoDefaultBtn->setEnabled(true);
			configPersonDefaultDirBtn->setEnabled(true);
		}
		else
		{
			openAutoDefaultBtn->setEnabled(false);
			configPersonDefaultDirBtn->setEnabled(false);
		}
	});

	(void)connect(openAutoDefaultBtn, &QPushButton::clicked, this, [&]() {
		QString absolutePath = QDir::toNativeSeparators(QDir(KGlobalDataNameSpace::getFatherDirName()
			+ "\\wpsgogsgit\\default").absolutePath());
		QDesktopServices::openUrl(QUrl::fromLocalFile(absolutePath));
	});

	(void)connect(configPersonDefaultDirBtn, &QPushButton::clicked, this, &KCreateStoreWin::onHandlePersonDefaultBtn);
}

void KCreateStoreWin::onHandleCreateRepoBtn()
{
	initTimerSig();
	KGitProcess gitProcess;
	gitProcess.createRepo(m_pLineEdit->text(), QString::number(m_pIsPrivateRepo->isChecked()),
		QString::number(m_pIsAddDefaultFile->isChecked()), KGlobalDataNameSpace::m_personFilePath);
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	clearTempFolder();
	createRepoSig(gitProcess.getOutput());
}

void KCreateStoreWin::onHandleBindUserAndRepoBtn()
{
	initTimerSig();
	KGitProcess gitProcess;
	gitProcess.bindUserAndRepo(m_pLineEdit->text());
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	bindUserAndRepoSig(gitProcess.getOutput());
}

void KCreateStoreWin::onHandleRepoInsertBtn()
{
	QString filePath = KGlobalDataNameSpace::getFatherDirName() + "/wpsgogsgit/venv/repoList.csv";
	QFile file(filePath);
	if (!file.exists())
	{
		QMessageBox::warning(Q_NULLPTR, QStringLiteral("fileNotExist"), tr("fileNotExistTr"), QMessageBox::Ok);
		return;
	}
	QUrl url = QUrl::fromLocalFile(filePath);
	QDesktopServices::openUrl(url);
}

void KCreateStoreWin::clearTempFolder()
{
	QString folderPath = KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_repoTempDir;
	QDir jplagTestClearDir(folderPath);
	if (jplagTestClearDir.exists())
		jplagTestClearDir.removeRecursively();

	QDir jplagTestCreateDir(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_repoTempTopDir);
	if (!jplagTestCreateDir.mkdir("temp"))
	{
		QMessageBox::warning(Q_NULLPTR, QStringLiteral("subDirCreateFailed"), QObject::tr("subDirCreateFailedTr"), QMessageBox::Ok);
		return;
	}
}

void KCreateStoreWin::onHandlePersonDefaultBtn()
{
	m_pToolWin = std::make_unique<KToolWin>("configPersonFilePath", tr("personDefaultDirLabelTr"));
	m_pToolWin->show();
	(void)connect(m_pToolWin.get(), &KToolWin::configPersonDefaultFilePathEndSignal, this, [&]() {
		emit configPersonDefaultFilePathEndSignal();
	});
}