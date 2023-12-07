#include "kdeletewin.h"
#include "kglobaldata.h"
#include "kgitprocess.h"
#include "krallfile.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCoreApplication>

KDeleteWin::KDeleteWin(QWidget *parent)
	: QWidget(parent)
	, m_pLineEdit(nullptr)
	, m_pTableView(nullptr)
	, m_pModel(nullptr)
	, m_pDelOrgRepoBtn(nullptr)
	, m_pDelOrgUserBtn(nullptr)
{
	initModel();
	initWindow();
}

KDeleteWin::~KDeleteWin()
{
	if (m_pModel != nullptr)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void KDeleteWin::initModel()
{
	m_pModel = new QStandardItemModel(0, 4);
	m_pModel->setHeaderData(0, Qt::Horizontal, tr("repoName"));
	m_pModel->setHeaderData(1, Qt::Horizontal, tr("userName"));
	m_pModel->setHeaderData(2, Qt::Horizontal, tr("account"));
	m_pModel->setHeaderData(3, Qt::Horizontal, tr("email"));
}

void KDeleteWin::initWindow()
{
	QLabel* titleLabel = new QLabel(tr("titleLabelTr"), this);
	QFont titlefont = titleLabel->font();
	titlefont.setPointSize(16);
	titleLabel->setFont(titlefont);
	titleLabel->setAlignment(Qt::AlignCenter);
	
	QLabel* orgLabel = new QLabel(tr("orgLabelTr") + ":", this);
	QFont font;
	font.setPointSize(12);
	orgLabel->setFont(font);
	m_pLineEdit = new QLineEdit(this);

	QPushButton* listOrgRepoUserBtn = new QPushButton(tr("listOrgRepoUserBtnTr"), this);
	m_pDelOrgRepoBtn = new QPushButton(tr("delOrgRepoBtnTr"), this);
	m_pDelOrgUserBtn = new QPushButton(tr("delOrgUserBtnTr"), this);
	QPushButton* delBackMainWinBtn = new QPushButton(QIcon(":/icons/main.png"), tr("backMainWinTr"), this);
	listOrgRepoUserBtn->setEnabled(false);
	m_pDelOrgRepoBtn->setEnabled(false);
	m_pDelOrgUserBtn->setEnabled(false);

	QHBoxLayout* m_pHOrgLay = new QHBoxLayout;
	m_pHOrgLay->addWidget(orgLabel);
	m_pHOrgLay->addWidget(m_pLineEdit);

	QHBoxLayout* m_pHBtnLay = new QHBoxLayout;
	listOrgRepoUserBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pDelOrgRepoBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pDelOrgUserBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	delBackMainWinBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pHBtnLay->addWidget(listOrgRepoUserBtn);
	m_pHBtnLay->addWidget(m_pDelOrgRepoBtn);
	m_pHBtnLay->addWidget(m_pDelOrgUserBtn);
	m_pHBtnLay->addWidget(delBackMainWinBtn);

	m_pTableView = new QTableView(this);
	m_pTableView->setEditTriggers(QTableView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);      //以拉伸填满整个表格
	m_pTableView->setModel(m_pModel);

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->setMargin(0);
	m_pVMainLay->setSpacing(0);
	m_pVMainLay->setAlignment(Qt::AlignTop);
	m_pVMainLay->addWidget(titleLabel);
	m_pVMainLay->addLayout(m_pHOrgLay);
	m_pVMainLay->addLayout(m_pHBtnLay);
	m_pVMainLay->addWidget(m_pTableView);

	(void)connect(delBackMainWinBtn, &QPushButton::clicked, this, [&]() {
		emit delBackMainWinSig();
	});
	(void)connect(m_pLineEdit, &QLineEdit::textChanged, this, [=]() {
		listOrgRepoUserBtn->setEnabled(!m_pLineEdit->text().isEmpty());
		if (m_pLineEdit->text().isEmpty())
		{
			m_pDelOrgRepoBtn->setEnabled(false);
			m_pDelOrgUserBtn->setEnabled(false);
		}
	});
	(void)connect(listOrgRepoUserBtn, &QPushButton::clicked, this, &KDeleteWin::onHandleListRepoUser);
	(void)connect(m_pDelOrgRepoBtn, &QPushButton::clicked, this, &KDeleteWin::showDelRepoDialog);
	(void)connect(m_pDelOrgUserBtn, &QPushButton::clicked, this, &KDeleteWin::showDelUserDialog);
}

void KDeleteWin::showDelRepoDialog()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this, tr("okTr"),
		tr("delRepoQuesTr") + "?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
		onHandleDelRepoBtn();
}

void KDeleteWin::showDelUserDialog()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this, tr("okTr"),
		tr("delUserQuesTr") + "?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
		onHandleDelUserBtn();
}

void KDeleteWin::onHandleListRepoUser()
{
	emit initTimerSig();
	KGitProcess gitProcess;
	gitProcess.delRepoAndUser(m_pLineEdit->text(), "listRepoUser");
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	insertTableView();
}

void KDeleteWin::onHandleDelRepoBtn()
{
	emit initTimerSig();
	KGitProcess gitProcess;
	gitProcess.delRepoAndUser(m_pLineEdit->text(), "deleteRepo");
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	int rowCount = m_pModel->rowCount();
	for (int row = 0; row < rowCount; ++row)
	{
		QModelIndex index = m_pModel->index(row, 0);
		m_pModel->setData(index, QVariant(), Qt::EditRole);
	}

	emit delRepoSig(gitProcess.getOutput());
}

void KDeleteWin::onHandleDelUserBtn()
{
	emit initTimerSig();
	KGitProcess gitProcess;
	gitProcess.delRepoAndUser(m_pLineEdit->text(), "deleteUser");
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	for (int row = 0; row < m_pModel->rowCount(); ++row)
	{
		QModelIndex index = m_pModel->index(row, 1);
		QModelIndex telIndex = m_pModel->index(row, 2);
		QModelIndex emailIndex = m_pModel->index(row, 3);
		m_pModel->setData(index, QVariant(), Qt::EditRole);
		m_pModel->setData(telIndex, QVariant(), Qt::EditRole);
		m_pModel->setData(emailIndex, QVariant(), Qt::EditRole);
	}

	emit delUserSig(gitProcess.getOutput());
}

void KDeleteWin::insertTableView()
{
	m_pModel->removeRows(0, m_pModel->rowCount());

	KRallFile file(KGlobalDataNameSpace::getFatherDirName() + "\\wpsgogsgit\\venv\\delete.csv", "read");
	QTextStream in(&(file.getFile()));
	in.readLine();

	while (!in.atEnd())
	{
		QStringList fields = in.readLine().split(",");
		QStandardItem* repoItem = new QStandardItem(fields[0]);
		QStandardItem* userItem = new QStandardItem(fields[1]);
		QStandardItem* accountItem = new QStandardItem(fields[2]);
		QStandardItem* emailItem = new QStandardItem(fields[3]);

		m_pModel->appendRow({ repoItem,userItem ,accountItem,emailItem });
		QCoreApplication::processEvents();
	}

	m_pDelOrgRepoBtn->setEnabled(true);
	m_pDelOrgUserBtn->setEnabled(true);
}
