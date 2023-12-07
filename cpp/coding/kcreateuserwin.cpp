#include "kcreateuserwin.h"
#include "kgitprocess.h"
#include "kglobaldata.h"
#include "krallfile.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTextStream>
#include <QMessageBox>
#include <QCoreApplication>

KCreateUserWin::KCreateUserWin(QWidget *parent)
	: QWidget(parent)
	, m_pCreateUserOkBtn(nullptr)
	, m_pTableView(nullptr)
	, m_pModel(nullptr)
{
	initModel();
	initWindow();
}

KCreateUserWin::~KCreateUserWin()
{
	if (m_pModel != nullptr)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
}

void KCreateUserWin::initModel()
{
	m_pModel = new QStandardItemModel(0, 5);
	m_pModel->setHeaderData(0, Qt::Horizontal, tr("name"));
	m_pModel->setHeaderData(1, Qt::Horizontal, tr("password"));
	m_pModel->setHeaderData(2, Qt::Horizontal, tr("account"));
	m_pModel->setHeaderData(3, Qt::Horizontal, tr("repoName"));
	m_pModel->setHeaderData(4, Qt::Horizontal, tr("check"));
}

void KCreateUserWin::initWindow()
{
	QLabel* createUserTitleLabel = new QLabel(tr("createUserTitleLabelTr"), this);
	QFont userfont = createUserTitleLabel->font();
	userfont.setPointSize(16);
	createUserTitleLabel->setFont(userfont);
	createUserTitleLabel->setAlignment(Qt::AlignCenter);
	QPushButton* userInsertBtn = new QPushButton(tr("userInsertBtnTr"), this);
	m_pCreateUserOkBtn = new QPushButton(tr("createUserOkBtnTr"), this);
	QPushButton* createSignalUserBtn = new QPushButton(tr("createSignalUserBtnTr"), this);
	QPushButton* userBackMainWinBtn = new QPushButton(QIcon(":/icons/main.png"), tr("backMainWinTr"), this);

	m_pTableView = new QTableView(this);
	m_pTableView->setEditTriggers(QTableView::NoEditTriggers);
	m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);      //以拉伸填满整个表格
	m_pTableView->setModel(m_pModel);

	QHBoxLayout* createUserLineLay = new QHBoxLayout;
	userInsertBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pCreateUserOkBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	createSignalUserBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	userBackMainWinBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	createUserLineLay->addWidget(userInsertBtn);
	createUserLineLay->addWidget(m_pCreateUserOkBtn);
	createUserLineLay->addWidget(createSignalUserBtn);
	createUserLineLay->addWidget(userBackMainWinBtn);

	QVBoxLayout* m_pVUserMainLay = new QVBoxLayout(this);
	m_pVUserMainLay->setMargin(0);              
	m_pVUserMainLay->setSpacing(0);           
	m_pVUserMainLay->setAlignment(Qt::AlignTop);
	m_pVUserMainLay->setAlignment(Qt::AlignTop);
	m_pVUserMainLay->addWidget(createUserTitleLabel);
	m_pVUserMainLay->addLayout(createUserLineLay);
	m_pVUserMainLay->addWidget(m_pTableView);

	(void)connect(m_pCreateUserOkBtn, &QPushButton::clicked, this, &KCreateUserWin::onHandleCreateUserBtn);
	(void)connect(userInsertBtn, &QPushButton::clicked, this, &KCreateUserWin::onHandleUserInsertBtn);

	(void)connect(userBackMainWinBtn, &QPushButton::clicked, this, [&]() {
		emit userBackMainWinSig();
	});
	(void)connect(createSignalUserBtn, &QPushButton::clicked, this, [&]() {
		QUrl url("https://git.wpsit.cn/admin/users/new");
		QDesktopServices::openUrl(url);
	});
}

void KCreateUserWin::onHandleCreateUserBtn()
{
	initTimerSig();
	KGitProcess gitProcess;
	gitProcess.createUser();
	gitProcess.start();
	while (!gitProcess.isFinished())
	{
		QCoreApplication::processEvents();
	}
	endTimerSig();

	createUserSig(gitProcess.getOutput());
	insertTableView();
}

void KCreateUserWin::insertTableView()
{
	m_pModel->removeRows(0, m_pModel->rowCount());

	KRallFile file(KGlobalDataNameSpace::getFatherDirName() + "\\wpsgogsgit\\venv\\resCsvFile.csv", "read");
	QTextStream in(&file.getFile());
	in.readLine();

	while (!in.atEnd())
	{
		QStringList fields = in.readLine().split(",");
		QStandardItem* nameItem = new QStandardItem(fields[0]);
		QStandardItem* passWordItem = new QStandardItem(fields[1]);
		QStandardItem* accountItem = new QStandardItem(fields[2]);
		QStandardItem* repoItem = new QStandardItem(fields[3]);
		QStandardItem* checkItem = new QStandardItem(fields[4]);

		m_pModel->appendRow({ nameItem,passWordItem ,accountItem,repoItem,checkItem });
		QCoreApplication::processEvents();
	}
}

void KCreateUserWin::onHandleUserInsertBtn()
{
	QString filePath = KGlobalDataNameSpace::getFatherDirName() + "\\wpsgogsgit\\venv\\userList.csv";
	QFile file(filePath);
	if (!file.exists())
	{
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(Q_NULLPTR, QStringLiteral("createFailed"), tr("createCsvFailedTr"), QMessageBox::Ok);
			return;
		}
		QTextStream out(&file);
		out << tr("Header1") << "," << tr("Header2") << "," << QObject::tr("Header3") << endl;

		file.close();
	}
	QUrl url = QUrl::fromLocalFile(filePath);
	QDesktopServices::openUrl(url);
}