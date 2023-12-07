#include "ktoolwin.h"
#include "kglobaldata.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

KToolWin::KToolWin(const QString& select, const QString& title, QWidget *parent)
	: QWidget(parent)
	, m_title(title)
	, m_select(select)
	, m_pLineEdit(nullptr)
{
	initWindow();
}

KToolWin::~KToolWin()
{
}

void KToolWin::initWindow()
{
	setMinimumSize(400, 200);

	QLabel* titleLabel = new QLabel(m_title, this);
	QFont titleFont;
	titleFont.setPointSize(16);
	titleLabel->setFont(titleFont);
	titleLabel->setAlignment(Qt::AlignCenter);
	QLabel* dirLabel = new QLabel(tr("dirLabel") + ":", this);
	QFont font;
	font.setPointSize(12);
	dirLabel->setFont(font);

	m_pLineEdit = new QLineEdit(this);
	if (m_select == "setOpenDirDefaultPath")
	{
		if (KGlobalDataNameSpace::getOpenDirDefaultPath() != QStandardPaths::DocumentsLocation)
			m_pLineEdit->setText(KGlobalDataNameSpace::getOpenDirDefaultPath());
	}
	else if (m_select == "configPersonFilePath")
	{
		m_pLineEdit->setText(KGlobalDataNameSpace::m_personFilePath);
	}
		

	QPushButton* dirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), this);
	QPushButton* okBtn = new QPushButton(tr("setOpenDirDefaultPathOkBtnTr"), this);
	okBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	okBtn->setEnabled(false);

	QHBoxLayout* setOpenDirDefaultPathLineLay = new QHBoxLayout;
	setOpenDirDefaultPathLineLay->addWidget(dirLabel);
	setOpenDirDefaultPathLineLay->addWidget(m_pLineEdit);
	setOpenDirDefaultPathLineLay->addWidget(dirBtn);

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->setAlignment(Qt::AlignTop);
	m_pVMainLay->addWidget(titleLabel);
	m_pVMainLay->addLayout(setOpenDirDefaultPathLineLay);
	m_pVMainLay->addWidget(okBtn);

	(void)connect(dirBtn, &QPushButton::clicked, this, [=]() {
		QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
		if (folderPath.isEmpty())
			return;
		m_pLineEdit->setText(folderPath);
	});

	(void)connect(okBtn, &QPushButton::clicked, this, &KToolWin::onHandleOpenDefaultDir);
	(void)connect(m_pLineEdit, &QLineEdit::textChanged, this, [=]() {
		okBtn->setEnabled(!(m_pLineEdit->text().isEmpty()));
	});
}

void KToolWin::onHandleOpenDefaultDir()
{
	if (m_select == "setOpenDirDefaultPath")
	{
		QSettings settings(KGlobalDataNameSpace::m_registerTablePath, QSettings::NativeFormat);
		settings.setValue(KGlobalDataNameSpace::m_registerTableDirDefaultOpenPathKey, m_pLineEdit->text());      
	}
	else if (m_select == "configPersonFilePath")
	{
		KGlobalDataNameSpace::m_personFilePath = m_pLineEdit->text();
		emit configPersonDefaultFilePathEndSignal();
	}
	this->close();
}