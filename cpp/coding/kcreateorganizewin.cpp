#include "kcreateorganizewin.h"
#include "kgitprocess.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QCoreApplication>
#include <QRegExpValidator>

KCreateOrganizeWin::KCreateOrganizeWin(QWidget *parent)
	: QWidget(parent)
	, m_pOrgLine(nullptr)
	, m_pOrgDescribeEdit(nullptr)
	, m_pCreateOrganizeOkBtn(nullptr)
{
	initWindow();
}

KCreateOrganizeWin::~KCreateOrganizeWin()
{
}

void KCreateOrganizeWin::initWindow()
{
	QLabel* createOrganizeTitleLabel = new QLabel(tr("createOrganizeTitleLabelTr"), this);
	QFont font = createOrganizeTitleLabel->font();
	font.setPointSize(16);
	createOrganizeTitleLabel->setFont(font);
	createOrganizeTitleLabel->setAlignment(Qt::AlignCenter);
	QFont labFont;
	labFont.setPointSize(12);
	QLabel* createOrganizeLabel = new QLabel(tr("createOrganizeLabelTr") + ":", this);
	createOrganizeLabel->setFont(labFont);
	m_pOrgLine = new QLineEdit(this);
	QRegExp regex("[^\u4e00-\u9fa5]+");     //取反中文字符Unicode范围
	QValidator* validator = new QRegExpValidator(regex, m_pOrgLine); 
	m_pOrgLine->setValidator(validator);    //设置验证器
	QLabel* orgDescribeLabel = new QLabel(tr("orgDescribeLabelTr") + ":", this);
	orgDescribeLabel->setFont(labFont);
	m_pOrgDescribeEdit = new QPlainTextEdit(this);
	QPushButton* organizeDuplicateNameQueryBtn = new QPushButton(tr("duplicateNameQueryBtnTr"), this);
	organizeDuplicateNameQueryBtn->setEnabled(false);
	m_pCreateOrganizeOkBtn = new QPushButton(tr("createOrganizeOkBtnTr"), this);
	m_pCreateOrganizeOkBtn->setEnabled(false);
	QPushButton* organizeBackMainWinBtn = new QPushButton(QIcon(":/icons/main.png"), tr("backMainWinTr"), this);

	QGridLayout* m_pOrgGridLay = new QGridLayout(this);
	m_pOrgGridLay->addWidget(createOrganizeTitleLabel, 0, 1, 1, 4);
	m_pOrgGridLay->addWidget(createOrganizeLabel, 1, 0, 2, 1);
	m_pOrgGridLay->addWidget(m_pOrgLine, 1, 1, 2, 4);
	m_pOrgGridLay->addWidget(orgDescribeLabel, 3, 0, 2, 1);
	m_pOrgGridLay->addWidget(m_pOrgDescribeEdit, 3, 1, 2, 4);
	m_pOrgGridLay->addWidget(organizeDuplicateNameQueryBtn, 5, 0, 2, 2);
	m_pOrgGridLay->addWidget(m_pCreateOrganizeOkBtn, 5, 2, 2, 2);
	m_pOrgGridLay->addWidget(organizeBackMainWinBtn, 5, 4, 2, 2);

	(void)connect(m_pOrgLine, &QLineEdit::textChanged, this, [=]() {
		organizeDuplicateNameQueryBtn->setEnabled(!m_pOrgLine->text().isEmpty());
		if (m_pOrgLine->text().isEmpty())
			m_pCreateOrganizeOkBtn->setEnabled(false);
	});

	(void)connect(organizeBackMainWinBtn, &QPushButton::clicked, this, [&]() {
		emit orgBackMainWinSig();
	});

	(void)connect(m_pCreateOrganizeOkBtn, &QPushButton::clicked, this, &KCreateOrganizeWin::onHandleCreateOrg);
	(void)connect(organizeDuplicateNameQueryBtn, &QPushButton::clicked, this, &KCreateOrganizeWin::onHandleOrganizeDuplicateNameQueryBtn);
}

void KCreateOrganizeWin::onHandleCreateOrg()
{
	initTimerSig();
	KGitProcess gitProcess;
	gitProcess.createOrg(m_pOrgLine->text(), m_pOrgDescribeEdit->toPlainText());
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}	
	endTimerSig();

	QString output = gitProcess.getOutput();

	if (output.split("\n").value(1).trimmed() == "201")
		emit createOrgSig(true, output);
	else
		emit createOrgSig(false, output);
}

void KCreateOrganizeWin::onHandleOrganizeDuplicateNameQueryBtn()
{
	initTimerSig();
	KGitProcess gitProcess;
	gitProcess.checkOrgNameExist(m_pOrgLine->text());
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();
	QString output = gitProcess.getOutput();

	if (output.trimmed() == "exist")
	{
		emit orgDuplicateNameSig(true);     // 组织名已存在
	}
	else
	{
		emit orgDuplicateNameSig(false);    // 组织名不存在
		m_pCreateOrganizeOkBtn->setEnabled(true);
	}
}





