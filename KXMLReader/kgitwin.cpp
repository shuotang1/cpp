#include "kgitwin.h"
#include "kglobaldata.h"
#include "kpullcodewin.h"
#include "kcreateorganizewin.h"
#include "kcreateuserwin.h"
#include "kcreatestorewin.h"
#include "kdeletewin.h"
#include "kdatabase.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QDateTime>
#include <QFormLayout>
#include <QSignalMapper>

KGitWin::KGitWin(QWidget *parent)
	: QWidget(parent)
	, m_pGitStackWin(nullptr)
	, m_pTextEdit(nullptr)
	, m_pProgressBar(nullptr)
	, m_pTimer(nullptr)
	, m_pRootUrlEdit(nullptr)
	, m_pAccessTokenEdit(nullptr)
	, m_pOrgWinChangeBtn(nullptr)
	, m_pUserWinChangeBtn(nullptr)
	, m_pRepoWinChangeBtn(nullptr)
	, m_pPullWinChangeBtn(nullptr)
	, m_pDelWinChangeBtn(nullptr)
{
	initWindow();
	initConfig();
}

KGitWin::~KGitWin()
{
}

void KGitWin::initWindow()
{
	m_pGitStackWin = new QStackedWidget(this);
	// 主窗口
	QWidget* mainWin = new QWidget(this);
	QPushButton* envConfigBtn = new QPushButton(tr("envConfigBtnTr"), mainWin);
	m_pOrgWinChangeBtn = new QPushButton(QIcon(":/icons/organize.png"), tr("organizeWinChangeBtnTr"), mainWin);
	m_pUserWinChangeBtn = new QPushButton(QIcon(":/icons/user.png"), tr("userWinChangeBtnTr"), mainWin);
	m_pRepoWinChangeBtn = new QPushButton(QIcon(":/icons/subStore.png"), tr("storeWinChangeBtnTr"), mainWin);
	m_pPullWinChangeBtn = new QPushButton(QIcon(":/icons/download.png"), tr("pullWinChangeBtnTr"), mainWin);
	m_pDelWinChangeBtn = new QPushButton(QIcon(":/icons/del.png"), tr("delWinChangeBtnTr"), mainWin);

	m_pOrgWinChangeBtn->setEnabled(false);
	m_pUserWinChangeBtn->setEnabled(false);
	m_pRepoWinChangeBtn->setEnabled(false);
	m_pPullWinChangeBtn->setEnabled(false);
	m_pDelWinChangeBtn->setEnabled(false);
	QHBoxLayout* m_pHChangeMainLay = new QHBoxLayout(mainWin);
	m_pHChangeMainLay->setMargin(0);
	m_pHChangeMainLay->setSpacing(0);
	envConfigBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pOrgWinChangeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pUserWinChangeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pRepoWinChangeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pPullWinChangeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pDelWinChangeBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pHChangeMainLay->addWidget(envConfigBtn);
	m_pHChangeMainLay->addWidget(m_pOrgWinChangeBtn);
	m_pHChangeMainLay->addWidget(m_pUserWinChangeBtn);
	m_pHChangeMainLay->addWidget(m_pRepoWinChangeBtn);
	m_pHChangeMainLay->addWidget(m_pPullWinChangeBtn);
	m_pHChangeMainLay->addWidget(m_pDelWinChangeBtn);

	QSignalMapper* signalMapper = new QSignalMapper(this);
	signalMapper->setMapping(envConfigBtn, 5);
	signalMapper->setMapping(m_pOrgWinChangeBtn, 1);
	signalMapper->setMapping(m_pUserWinChangeBtn, 2);
	signalMapper->setMapping(m_pRepoWinChangeBtn, 3);
	signalMapper->setMapping(m_pPullWinChangeBtn, 4);
	signalMapper->setMapping(m_pDelWinChangeBtn, 6);
	(void)connect(signalMapper, QOverload<int>::of(&QSignalMapper::mapped), this, &KGitWin::onHandleChangeWinBtn);

	(void)connect(envConfigBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pOrgWinChangeBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pUserWinChangeBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pRepoWinChangeBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pPullWinChangeBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pDelWinChangeBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));

	// 配置窗口
	QWidget* envConfigWin = new QWidget(this);
	QLabel* rootUrlLab = new QLabel(tr("rootUrlLab") + ":", envConfigWin);
	QLabel* accessTokenLab = new QLabel(tr("accessTokenLab") + ":", envConfigWin);
	QFont font;
	font.setPointSize(12);
	rootUrlLab->setFont(font);
	accessTokenLab->setFont(font);
	m_pRootUrlEdit = new QLineEdit(envConfigWin);
	m_pRootUrlEdit->setText("https://git.wpsit.cn/");
	m_pRootUrlEdit->setPlaceholderText("https://git.wpsit.cn/");
	m_pAccessTokenEdit = new QLineEdit(envConfigWin);
	QPushButton* configBtn = new QPushButton(tr("configBtnTr"), envConfigWin);
	configBtn->setEnabled(false);
	QPushButton* envBackMainBtn = new QPushButton(QIcon(":/icons/main.png"), tr("backMainWinTr"), envConfigWin);
	QFormLayout* headerLayout = new QFormLayout;
	headerLayout->setSpacing(10);
	headerLayout->addRow(rootUrlLab, m_pRootUrlEdit);
	headerLayout->addRow(accessTokenLab, m_pAccessTokenEdit);
	QHBoxLayout* m_pHEnvBtnLay = new QHBoxLayout;
	configBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	envBackMainBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pHEnvBtnLay->addWidget(configBtn);
	m_pHEnvBtnLay->addWidget(envBackMainBtn);
	QVBoxLayout* m_pVEnvMainLay = new QVBoxLayout(envConfigWin);
	m_pVEnvMainLay->addLayout(headerLayout);
	m_pVEnvMainLay->addLayout(m_pHEnvBtnLay);

	KCreateOrganizeWin* createOrganizeWin = new KCreateOrganizeWin(m_pGitStackWin);
	KCreateUserWin* createUserWin = new KCreateUserWin(m_pGitStackWin);
	KCreateStoreWin* createStoreWin = new KCreateStoreWin(m_pGitStackWin);
	KPullCodeWin* m_pPullCodeWin = new KPullCodeWin(m_pGitStackWin);
	KDeleteWin* m_pDelWin = new KDeleteWin(m_pGitStackWin);

	signalMapper->setMapping(createOrganizeWin, 0);
	signalMapper->setMapping(createUserWin, 0);
	signalMapper->setMapping(createStoreWin, 0);
	signalMapper->setMapping(m_pPullCodeWin, 0);
	signalMapper->setMapping(envBackMainBtn, 0);
	signalMapper->setMapping(m_pDelWin, 0);

	m_pTextEdit = new QTextBrowser(this);
	m_pTextEdit->setReadOnly(true);
	m_pTextEdit->setOpenLinks(true);
	m_pTextEdit->setOpenExternalLinks(true);

	QLabel* progressLabel = new QLabel(tr("progressLabelTr") + ": ", this);
	m_pProgressBar = new QProgressBar(this);
	QHBoxLayout* m_pHProgressLay = new QHBoxLayout;
	m_pHProgressLay->addWidget(progressLabel);
	m_pHProgressLay->addWidget(m_pProgressBar);

	m_pTimer = new QTimer(this);

	m_pGitStackWin->addWidget(mainWin);
	m_pGitStackWin->addWidget(createOrganizeWin);
	m_pGitStackWin->addWidget(createUserWin);
	m_pGitStackWin->addWidget(createStoreWin);
	m_pGitStackWin->addWidget(m_pPullCodeWin);
	m_pGitStackWin->addWidget(envConfigWin);
	m_pGitStackWin->addWidget(m_pDelWin);

	QVBoxLayout* m_pHSimMainLay = new QVBoxLayout(this);
	m_pHSimMainLay->setMargin(0);              //设置外边距
	m_pHSimMainLay->setSpacing(0);             //设置内边距
	m_pHSimMainLay->setAlignment(Qt::AlignTop);
	m_pHSimMainLay->addWidget(m_pGitStackWin, 2);
	m_pHSimMainLay->addWidget(m_pTextEdit, 3);
	m_pHSimMainLay->addLayout(m_pHProgressLay);
	setLayout(m_pHSimMainLay);

	(void)connect(createOrganizeWin, &KCreateOrganizeWin::orgBackMainWinSig, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(createOrganizeWin, &KCreateOrganizeWin::createOrgSig, this, &KGitWin::onHandleCreateOrg);
	(void)connect(createOrganizeWin, &KCreateOrganizeWin::orgDuplicateNameSig, this, &KGitWin::onHandleOrganizeDuplicateNameQuery);
	(void)connect(createOrganizeWin, &KCreateOrganizeWin::initTimerSig, this, &KGitWin::initTimerStart);
	(void)connect(createOrganizeWin, &KCreateOrganizeWin::endTimerSig, this, &KGitWin::initTimerEnd);

	(void)connect(createUserWin, &KCreateUserWin::userBackMainWinSig, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(createUserWin, &KCreateUserWin::createUserSig, this, &KGitWin::onHandleCreateUser);
	(void)connect(createUserWin, &KCreateUserWin::initTimerSig, this, &KGitWin::initTimerStart);
	(void)connect(createUserWin, &KCreateUserWin::endTimerSig, this, &KGitWin::initTimerEnd);

	(void)connect(createStoreWin, &KCreateStoreWin::repoBackMainWinSig, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(createStoreWin, &KCreateStoreWin::createRepoSig, this, &KGitWin::onHandleCreateRepo);
	(void)connect(createStoreWin, &KCreateStoreWin::bindUserAndRepoSig, this, &KGitWin::onHandleBindUserAndRepo);
	(void)connect(createStoreWin, &KCreateStoreWin::initTimerSig, this, &KGitWin::initTimerStart);
	(void)connect(createStoreWin, &KCreateStoreWin::endTimerSig, this, &KGitWin::initTimerEnd);
	(void)connect(createStoreWin, &KCreateStoreWin::configPersonDefaultFilePathEndSignal, this, &KGitWin::personConfigDefault);

	(void)connect(m_pPullCodeWin, &KPullCodeWin::pullBackMainWinClick, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pPullCodeWin, &KPullCodeWin::initTimerSig, this, &KGitWin::initTimerStart);
	(void)connect(m_pPullCodeWin, &KPullCodeWin::endTimerSig, this, &KGitWin::initTimerEnd);
	(void)connect(m_pPullCodeWin, &KPullCodeWin::pullCodeSig, this, &KGitWin::onHandlePullCode);

	(void)connect(m_pDelWin, &KDeleteWin::delBackMainWinSig, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(m_pDelWin, &KDeleteWin::initTimerSig, this, &KGitWin::initTimerStart);
	(void)connect(m_pDelWin, &KDeleteWin::endTimerSig, this, &KGitWin::initTimerEnd);
	(void)connect(m_pDelWin, &KDeleteWin::delRepoSig, this, &KGitWin::onHandleDelRepo);
	(void)connect(m_pDelWin, &KDeleteWin::delUserSig, this, &KGitWin::onHandleDelUser);

	(void)connect(envBackMainBtn, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
	(void)connect(configBtn, &QPushButton::clicked, this, &KGitWin::onHandleConfigBtn);
	(void)connect(m_pAccessTokenEdit, &QLineEdit::textChanged, this, [=]() {
		configBtn->setEnabled(!(m_pAccessTokenEdit->text().isEmpty() || m_pRootUrlEdit->text().isEmpty()));
	});
	(void)connect(m_pRootUrlEdit, &QLineEdit::textChanged, this, [=]() {
		configBtn->setEnabled(!(m_pAccessTokenEdit->text().isEmpty() || m_pRootUrlEdit->text().isEmpty()));
	});

	(void)connect(m_pTimer, &QTimer::timeout, this, &KGitWin::onHandleTimeout);
}

void KGitWin::initConfig()
{
	KDataBase db;
	QVector<QString> configVec;
	db.selectConfig(configVec);
	if (!configVec.isEmpty())
	{
		m_pRootUrlEdit->setText(configVec[0]);
		m_pAccessTokenEdit->setText(configVec[1]);
	}
}

void KGitWin::onHandleCreateOrg(bool isSuccess, const QString& output)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginCreateOrgainzeTr") + "\n");

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(output);
	if (isSuccess)
	{
		QTextCharFormat purpleFormat;
		purpleFormat.setForeground(QColor("#800080")); 
		cursor.setCharFormat(purpleFormat);
		cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("orgUrlTr") + "\n");
		QTextCharFormat linkFormat;
		linkFormat.setAnchor(true); // 设置为超链接  
		linkFormat.setAnchorHref(output.split("\n").value(2).trimmed()); // 设置超链接的URL  
		cursor.insertHtml("<a href=\"" + output.split("\n").value(2).trimmed() + "\">" + tr("openOrgTr") + "</a>");
		QTextCharFormat normalFormat;
		normalFormat.setForeground(Qt::red);  
		cursor.setCharFormat(normalFormat);
		cursor.insertText("\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " +
			tr("createOrgainzeSuccessTr") + "\n");
	}	
	else
	{
		m_pTextEdit->setTextColor(Qt::red);
		m_pTextEdit->append(tr("createOrgainzeFailedTr") + "\n");
	}

	m_pTextEdit->setTextCursor(cursor);

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandleCreateUser(const QString& output)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginCreateUserTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(output);
	m_pTextEdit->setTextColor(Qt::red);
	m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("createUserSuccessTr") + "\n");
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandleCreateRepo(const QString& output)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginCreateStoreTr") + "\n");

	QTextCharFormat normalBlackFormat;
	normalBlackFormat.setForeground(Qt::black);
	cursor.setCharFormat(normalBlackFormat);
	cursor.insertText(output + "\n");

	QTextCharFormat purpleFormat;
	purpleFormat.setForeground(QColor("#800080"));
	cursor.setCharFormat(purpleFormat);
	cursor.insertText("\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("repoUrlTr") + "\n");

	QStringList lines = output.split("\n");
	QRegularExpression regex("(http|https)");
	for (const QString& line : lines)
	{
		if (line.contains(regex))
		{
			QTextCharFormat linkFormat;
			linkFormat.setAnchor(true); 
			linkFormat.setAnchorHref(line.trimmed());   
			cursor.insertHtml("<a href=\"" + line.trimmed() + "\">" + line.mid(line.lastIndexOf("/") + 1) + "</a>");
			cursor.insertText("\n");
		}
	}

	QTextCharFormat normalFormat;
	normalFormat.setForeground(Qt::red);
	cursor.setCharFormat(normalFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("createRepoEndTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandleBindUserAndRepo(const QString& output)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginBindUserAndRepoTr") + "\n");

	QTextCharFormat normalBlackFormat;
	normalBlackFormat.setForeground(Qt::black);
	cursor.setCharFormat(normalBlackFormat);
	cursor.insertText(output + "\n");

	QTextCharFormat purpleFormat;
	purpleFormat.setForeground(QColor("#800080"));
	cursor.setCharFormat(purpleFormat);
	cursor.insertText("\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("bindUserAndRepoUrlTr") + "\n");

	QStringList lines = output.split("\n");
	QRegularExpression regex("(http|https)");
	for (const QString& line : lines)
	{
		if (line.contains(regex))
		{
			QTextCharFormat linkFormat;
			linkFormat.setAnchor(true);
			linkFormat.setAnchorHref(line.trimmed());
			int start = line.indexOf("/", line.indexOf("/", line.indexOf("/", line.indexOf("/") + 1) + 1) + 1);
			int end = line.indexOf("/", start + 1);
			cursor.insertHtml("<a href=\"" + line.trimmed() + "\">" + line.mid(start + 1, end - start - 1) + "</a>");
			cursor.insertText("\n");
		}
	}

	QTextCharFormat normalFormat;
	normalFormat.setForeground(Qt::red);
	cursor.setCharFormat(normalFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("endBindUserAndRepoTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandleDelRepo(const QString& output)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginDelRepoTr") + "\n");
	
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(output);

	QTextCharFormat purpleFormat;
	purpleFormat.setForeground(QColor("#800080"));
	cursor.setCharFormat(purpleFormat);
	cursor.insertText("\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("delRepoOrgUrlTr") + "\n");

	QStringList lines = output.split("\n");
	QRegularExpression regex("(http|https)");
	for (const QString& line : lines)
	{
		if (line.contains(regex))
		{
			QTextCharFormat linkFormat;
			linkFormat.setAnchor(true);
			linkFormat.setAnchorHref(line.trimmed());
			cursor.insertHtml("<a href=\"" + line.trimmed() + "\">" + tr("delRepoOrgUrlLabelTr") + "</a>");
			cursor.insertText("\n");
		}
	}

	QTextCharFormat normalFormat;
	normalFormat.setForeground(Qt::red);
	cursor.setCharFormat(normalFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("endDelRepoTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandleDelUser(const QString& output)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginDelUserTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(output);
	m_pTextEdit->setTextColor(Qt::red);
	m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("endDelUserTr") + "\n");
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::personConfigDefault()
{
	m_pTextEdit->setTextColor(Qt::blue);
	m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("personConfigDefalut") + ":\n");
	m_pTextEdit->setTextColor(Qt::red);
	m_pTextEdit->append(KGlobalDataNameSpace::m_personFilePath + "\n");
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandlePullCode(const QString& output,const QString& dir)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("beginPullCodeTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(output);

	QTextCharFormat purpleFormat;
	purpleFormat.setForeground(QColor("#800080"));
	cursor.setCharFormat(purpleFormat);
	cursor.insertText("\n" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("openPullCodeOutDirTr") + "\n");

	QTextCharFormat linkFormat;
	linkFormat.setForeground(Qt::blue);
	linkFormat.setAnchor(true); 
	linkFormat.setAnchorHref(dir);
	cursor.insertText(dir, linkFormat);
	cursor.insertText("\n");

	QTextCharFormat normalFormat;
	normalFormat.setForeground(Qt::red);
	cursor.setCharFormat(normalFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("endPullCodeTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);

	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
}

void KGitWin::onHandleOrganizeDuplicateNameQuery(bool isExist)
{
	QTextCursor cursor = m_pTextEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	QTextCharFormat normalBlueFormat;
	normalBlueFormat.setForeground(Qt::blue);
	cursor.setCharFormat(normalBlueFormat);
	cursor.insertText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + 
		tr("beginOrganizeDuplicateNameQueryTr") + "\n");
	m_pTextEdit->setTextCursor(cursor);
	if (isExist)
	{
		m_pTextEdit->setTextColor(Qt::red);
		m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("organizeDuplicateNameQuerySuccessTr") + "\n");
	}
	else
	{
		m_pTextEdit->setTextColor(Qt::red);
		m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("organizeDuplicateNameQueryFailedTr") + "\n");
	}
}

void KGitWin::onHandleChangeWinBtn(int index)
{
	m_pGitStackWin->setCurrentIndex(index);
}

void KGitWin::initTimerStart()
{
	m_pProgressBar->setMinimum(0);
	m_pProgressBar->setMaximum(0);  
	m_pProgressBar->setValue(0);
	m_pTimer->start(100);
}

void KGitWin::onHandleTimeout()
{
	int value = m_pProgressBar->value();
	m_pProgressBar->setValue(value + 1);
}

void KGitWin::initTimerEnd()
{
	m_pTimer->stop();
	m_pProgressBar->setMaximum(100);
	m_pProgressBar->setValue(100);
}

void KGitWin::onHandleConfigBtn()
{
	KGlobalDataNameSpace::m_rootUrl = m_pRootUrlEdit->text();
	KGlobalDataNameSpace::m_token = m_pAccessTokenEdit->text();
	KDataBase db;
	QVector<QString> config{ KGlobalDataNameSpace::m_rootUrl,KGlobalDataNameSpace::m_token };
	db.clearConfig();
	db.insertConfig(config);
	m_pOrgWinChangeBtn->setEnabled(true);
	m_pUserWinChangeBtn->setEnabled(true);
	m_pRepoWinChangeBtn->setEnabled(true);
	m_pPullWinChangeBtn->setEnabled(true);
	m_pDelWinChangeBtn->setEnabled(true);
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   root_url:" + m_pRootUrlEdit->text() + "\n");
	m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   token:" + m_pAccessTokenEdit->text() + "\n");
	m_pTextEdit->setTextColor(Qt::red);
	m_pTextEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ":   " + tr("EnvConfigSuccessTr") + "\n");
	m_pTextEdit->setTextColor(Qt::black);
	m_pTextEdit->append(tr("resultSymbolTr") + "\n");
	onHandleChangeWinBtn(0);
}