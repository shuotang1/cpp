// --------------------------------------------------------------
// ksavewindow.cpp
// 创建时间:2023/10/26
// 功能描述：保存为csv文件窗口
// ----------------------------------------------------------------
#include "ksavewindow.h"
#include "kglobaldata.h"
#include "kdatabase.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFormLayout>

KSaveWindow::KSaveWindow(QWidget* parent)
	: QWidget(parent)
	, m_pDirectoryLine(nullptr)
	, m_pReportFormatBox(nullptr)
{
	initWindow();
}

KSaveWindow::~KSaveWindow()
{
}

void KSaveWindow::initWindow()
{
	QFont font;
	font.setPointSize(12);
	QLabel* m_pFileNameLabel = new QLabel(tr("fileNameLabelTr") + ":", this);
	m_pFileNameLabel->setFont(font);
	QLabel* m_pReportFormat = new QLabel(tr("reportFormatLabelTr") + ": ", this);
	m_pReportFormat->setFont(font);

	QPushButton* m_pDirectoryBtn = new QPushButton(QIcon(":/icons/folder.png"), "", this);
	QPushButton* m_pSavBtn = new QPushButton(tr("saveTr"), this);
	m_pSavBtn->setEnabled(false);
	QPushButton* m_pCancelBtn = new QPushButton(tr("cancelTr"), this);
	m_pSavBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pCancelBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_pDirectoryLine = new QLineEdit(this);

	m_pReportFormatBox = new QComboBox(this);
	m_pReportFormatBox->setFont(font);
	m_pReportFormatBox->addItem(tr("reportFormatBoxAddCheckTr"));
	m_pReportFormatBox->addItem(tr("scoreTr"));

	QHBoxLayout* m_pHFirFileNameLay = new QHBoxLayout;
	m_pHFirFileNameLay->addWidget(m_pDirectoryLine);
	m_pHFirFileNameLay->addWidget(m_pDirectoryBtn);

	QFormLayout* headerLayout = new QFormLayout;
	headerLayout->setSpacing(10);
	headerLayout->addRow(m_pFileNameLabel, m_pHFirFileNameLay);
	headerLayout->addRow(m_pReportFormat, m_pReportFormatBox);

	QHBoxLayout* m_pHThirBtnLay = new QHBoxLayout;
	m_pHThirBtnLay->addWidget(m_pSavBtn);
	m_pHThirBtnLay->addWidget(m_pCancelBtn);

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->addLayout(headerLayout);
	m_pVMainLay->addLayout(m_pHThirBtnLay);

	(void)connect(m_pDirectoryBtn, &QPushButton::clicked, this, &KSaveWindow::onHandleDirectoryOpen);
	(void)connect(m_pSavBtn, &QPushButton::clicked, this, &KSaveWindow::onHandleSaveClicked);
	(void)connect(m_pCancelBtn, &QPushButton::clicked, this, &KSaveWindow::onHandleCancleClicked);
	(void)connect(m_pDirectoryLine, &QLineEdit::textChanged, this, [=]() {
		m_pSavBtn->setEnabled(!m_pDirectoryLine->text().isEmpty());
	});
}

void KSaveWindow::onHandleDirectoryOpen()
{
	QString filePath = QFileDialog::getSaveFileName(this, tr("filePathTr"),KGlobalDataNameSpace::getOpenDirDefaultPath(), QStringLiteral("CSV(*.csv)"));
	if (filePath.isEmpty())
		return;
	m_pDirectoryLine->setText(filePath);
}

void KSaveWindow::onHandleSaveClicked()
{
	KDataBase db;
	bool ok = true;
	if (m_pReportFormatBox->currentIndex() == 0)
		ok = db.saveCSV("cppcheck", m_pDirectoryLine->text());
	else if (m_pReportFormatBox->currentIndex() == 1)
		ok = db.saveCSV("score", m_pDirectoryLine->text());
	if (!ok)
		return;
	QMessageBox::warning(Q_NULLPTR, QStringLiteral("saveCsvSuccess"), tr("saveCsvSuccessTr"), QMessageBox::Ok);
	m_pDirectoryLine->clear();
	onHandleCancleClicked();
}

void KSaveWindow::onHandleCancleClicked()
{
	this->hide();
}

void KSaveWindow::setSaveTable(int index)
{
	m_pReportFormatBox->setCurrentIndex(index);
}