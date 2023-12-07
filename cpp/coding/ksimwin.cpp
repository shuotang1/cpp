#include "ksimwin.h"
#include "kjplagsimilarity.h"
#include "kglobaldata.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QHeaderView>

KSimWin::KSimWin(QWidget *parent)
	: QWidget(parent)
	, m_pSimTotalDirLine(nullptr)
	, m_pSimIncludeLine(nullptr)
	, m_pSimTableView(nullptr)
	, m_pSimModel(nullptr)
	, m_pSimFirFileLine(nullptr)
	, m_pSimSecFileLine(nullptr)
	, m_pSimFileBtn(nullptr)
	, m_pSimStackWin(nullptr)
	, m_pSimSpecDirLine(nullptr)
	, m_pSimSpecBtn(nullptr)
	, m_pSimDirBtn(nullptr)
	, m_pSimFirDirLine(nullptr)
	, m_pSimSecDirLine(nullptr)
	, m_pSimSpecExcludeLine(nullptr)
	, m_pComboBox(nullptr)
{
	initSimilarityModel();
	initWin();
}

KSimWin::~KSimWin()
{
	if (m_pSimModel != nullptr)
	{
		delete m_pSimModel;
		m_pSimModel = nullptr;
	}
}

void KSimWin::initWin()
{
	m_pSimStackWin = new QStackedWidget(this);

	QWidget* totalWin = new QWidget(m_pSimStackWin);

	QLabel* simTotalLabel = new QLabel(tr("simTotalLabelTr") + ":", totalWin);
	simTotalLabel->setToolTip(tr("simTotalLabelToolTipTr"));
	m_pSimTotalDirLine = new QLineEdit(totalWin);
	QPushButton* simTotalDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), totalWin);
	QPushButton* similarityTotalBtn = new QPushButton(tr("simTotalBtnTr"), totalWin);
	similarityTotalBtn->setEnabled(false);
	similarityTotalBtn->setToolTip(tr("simTotalBtnToolTipTr"));

	QHBoxLayout* m_pHFirLineLay = new QHBoxLayout(totalWin);
	m_pHFirLineLay->addWidget(simTotalLabel);
	m_pHFirLineLay->addWidget(m_pSimTotalDirLine);
	m_pHFirLineLay->addWidget(simTotalDirBtn);
	m_pHFirLineLay->addWidget(similarityTotalBtn);
	totalWin->setLayout(m_pHFirLineLay);
	

	QWidget* simSpecWin = new QWidget(m_pSimStackWin);

	QLabel* simSpecLabel = new QLabel(tr("simSpecLabelTr") + ":", simSpecWin);
	simSpecLabel->setToolTip(tr("simSpecLabelToolTipTr"));
	m_pSimSpecDirLine = new QLineEdit(simSpecWin);
	QPushButton* simSpecDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), simSpecWin);
	QLabel* SimincludeLabel = new QLabel(tr("simIncludeLabelTr") + ":", simSpecWin);
	SimincludeLabel->setToolTip(tr("simIncludeLabelToolTipTr"));
	m_pSimIncludeLine = new QLineEdit(simSpecWin);
	m_pSimIncludeLine->setPlaceholderText(tr("simIncludeLinePlaceHolderTextTr"));
	QLabel* simSpecExcludeLabel = new QLabel(tr("simSpecExcludeLabel") + ":", simSpecWin);
	m_pSimSpecExcludeLine = new QLineEdit(simSpecWin);
	m_pSimSpecExcludeLine->setPlaceholderText("*.md;*.txt");
	m_pComboBox = new QComboBox(simSpecWin);
	m_pComboBox->addItem("cpp");
	m_pComboBox->addItem("text");
	QPushButton* simSpecSubDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), simSpecWin);
	m_pSimSpecBtn = new QPushButton(tr("simSpecBtnTr"), simSpecWin);
	m_pSimSpecBtn->setEnabled(false);
	m_pSimSpecBtn->setToolTip(tr("simSpecBtnToolTipTr"));

	QHBoxLayout* m_pHSecLineLay = new QHBoxLayout(simSpecWin);
	m_pHSecLineLay->addWidget(simSpecLabel);
	m_pHSecLineLay->addWidget(m_pSimSpecDirLine);
	m_pHSecLineLay->addWidget(simSpecDirBtn);
	m_pHSecLineLay->addWidget(SimincludeLabel);
	m_pHSecLineLay->addWidget(m_pSimIncludeLine);
	m_pHSecLineLay->addWidget(simSpecSubDirBtn);
	m_pHSecLineLay->addWidget(simSpecExcludeLabel);
	m_pHSecLineLay->addWidget(m_pSimSpecExcludeLine);
	m_pHSecLineLay->addWidget(m_pComboBox);
	m_pHSecLineLay->addWidget(m_pSimSpecBtn);
	simSpecWin->setLayout(m_pHSecLineLay);


	QWidget* compareSimWin = new QWidget(m_pSimStackWin);

	QLabel* simFirFileLabel = new QLabel(tr("simFirFileLabelTr") + ":", compareSimWin);
	QLabel* simSecFileLabel = new QLabel(tr("simSecFileLabelTr"), compareSimWin);
	m_pSimFirFileLine = new QLineEdit(compareSimWin);
	m_pSimSecFileLine = new QLineEdit(compareSimWin);
	QPushButton* simFirFileBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), compareSimWin);
	QPushButton* simSecFileBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), compareSimWin);
	m_pSimFileBtn = new QPushButton(tr("simFileBtnTr"), compareSimWin);
	m_pSimFileBtn->setEnabled(false);

	QHBoxLayout* m_pHFirFileLineLay = new QHBoxLayout(compareSimWin);
	m_pHFirFileLineLay->addWidget(simFirFileLabel);
	m_pHFirFileLineLay->addWidget(m_pSimFirFileLine);
	m_pHFirFileLineLay->addWidget(simFirFileBtn);
	m_pHFirFileLineLay->addWidget(simSecFileLabel);
	m_pHFirFileLineLay->addWidget(m_pSimSecFileLine);
	m_pHFirFileLineLay->addWidget(simSecFileBtn);
	m_pHFirFileLineLay->addWidget(m_pSimFileBtn);
	compareSimWin->setLayout(m_pHFirFileLineLay);


	QWidget* compareDirSimWin = new QWidget(m_pSimStackWin);

	QLabel* simFirDirLabel = new QLabel(tr("simFirDirLabelTr") + ":", compareDirSimWin);
	QLabel* simSecDirLabel = new QLabel(tr("simSecDirLabelTr"), compareDirSimWin);
	m_pSimFirDirLine = new QLineEdit(compareDirSimWin);
	m_pSimSecDirLine = new QLineEdit(compareDirSimWin);
	QPushButton* simFirDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), compareDirSimWin);
	QPushButton* simSecDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), compareDirSimWin);
	m_pSimDirBtn = new QPushButton(tr("simDirBtnTr"), compareDirSimWin);
	m_pSimDirBtn->setEnabled(false);

	QHBoxLayout* m_pHFirDirLineLay = new QHBoxLayout(compareDirSimWin);
	m_pHFirDirLineLay->addWidget(simFirDirLabel);
	m_pHFirDirLineLay->addWidget(m_pSimFirDirLine);
	m_pHFirDirLineLay->addWidget(simFirDirBtn);
	m_pHFirDirLineLay->addWidget(simSecDirLabel);
	m_pHFirDirLineLay->addWidget(m_pSimSecDirLine);
	m_pHFirDirLineLay->addWidget(simSecDirBtn);
	m_pHFirDirLineLay->addWidget(m_pSimDirBtn);

	m_pSimStackWin->addWidget(totalWin);
	m_pSimStackWin->addWidget(simSpecWin);
	m_pSimStackWin->addWidget(compareSimWin);
	m_pSimStackWin->addWidget(compareDirSimWin);

	m_pSimTableView = new QTableView(this);
	m_pSimTableView->setEditTriggers(QTableView::NoEditTriggers);
	m_pSimTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);      //以拉伸填满整个表格
	m_pSimTableView->setModel(m_pSimModel);

	QVBoxLayout* m_pHSimMainLay = new QVBoxLayout(this);
	m_pHSimMainLay->setMargin(0);              //设置外边距
	m_pHSimMainLay->setSpacing(0);             //设置内边距
	m_pHSimMainLay->setAlignment(Qt::AlignTop);
	m_pHSimMainLay->addWidget(m_pSimStackWin);
	m_pHSimMainLay->addWidget(m_pSimTableView, 4);
	setLayout(m_pHSimMainLay);

	(void)connect(simTotalDirBtn, &QPushButton::clicked, this, &KSimWin::handleSimTotalDirClick);
	(void)connect(simSpecDirBtn, &QPushButton::clicked, this, &KSimWin::handleSimSpecDirClick);
	(void)connect(simSpecSubDirBtn, &QPushButton::clicked, this, &KSimWin::handleSimSpecSubDirClick);

	(void)connect(simFirFileBtn, &QPushButton::clicked, this, &KSimWin::handleSimFirFileClick);
	(void)connect(simSecFileBtn, &QPushButton::clicked, this, &KSimWin::handleSimSecFileClick);
	(void)connect(simFirDirBtn, &QPushButton::clicked, this, &KSimWin::handleSimFirDirClick);
	(void)connect(simSecDirBtn, &QPushButton::clicked, this, &KSimWin::handleSimSecDirClick);

	(void)connect(similarityTotalBtn, &QPushButton::clicked, this, &KSimWin::handleSimTotalClick);
	(void)connect(m_pSimSpecBtn, &QPushButton::clicked, this, &KSimWin::handleSimSpecClick);
	(void)connect(m_pSimFileBtn, &QPushButton::clicked, this, &KSimWin::handleFileSimClick);
	(void)connect(m_pSimDirBtn, &QPushButton::clicked, this, &KSimWin::handleDirSimClick);

	(void)connect(m_pSimFirFileLine, &QLineEdit::textChanged, this, &KSimWin::onHandleFileBtnChange);
	(void)connect(m_pSimSecFileLine, &QLineEdit::textChanged, this, &KSimWin::onHandleFileBtnChange);
	(void)connect(m_pSimFirDirLine, &QLineEdit::textChanged, this, &KSimWin::onHandleDirBtnChange);
	(void)connect(m_pSimSecDirLine, &QLineEdit::textChanged, this, &KSimWin::onHandleDirBtnChange);
	(void)connect(m_pSimSpecDirLine, &QLineEdit::textChanged, this, &KSimWin::onHandleSpecBtnChange);
	(void)connect(m_pSimIncludeLine, &QLineEdit::textChanged, this, &KSimWin::onHandleSpecBtnChange);
	
	(void)connect(m_pSimTotalDirLine, &QLineEdit::textChanged, this, [=]() {
		similarityTotalBtn->setEnabled(!m_pSimTotalDirLine->text().isEmpty());
	});
}

void KSimWin::handleSimTotalClick()
{
	int rowCount = m_pSimModel->rowCount();
	m_pSimModel->removeRows(0, rowCount);

	KJplagSimilarity jplagSimilarity(m_pSimTotalDirLine->text());
	jplagSimilarity.processDirectory(m_pSimTotalDirLine->text().toStdString());

	QVector<SimilarityResults> comparisons;
	comparisons.reserve(100);    //预分配存储相似度容器大小为100
	jplagSimilarity.outputDirectorySimilarity(m_pSimTotalDirLine->text(), comparisons);

	for (const SimilarityResults& res : comparisons)
	{
		QStandardItem* firstNameItem = new QStandardItem(res.firstFileName);
		QStandardItem* secondNameItem = new QStandardItem(res.secondFileName);
		QStandardItem* similarityItem = new QStandardItem(res.similarity);

		m_pSimModel->appendRow({ firstNameItem,secondNameItem ,similarityItem });
		QCoreApplication::processEvents();
	}
}

void KSimWin::handleSimSpecClick()
{
	int rowCount = m_pSimModel->rowCount();
	m_pSimModel->removeRows(0, rowCount);

	KJplagSimilarity jplagSimilarity(m_pSimSpecDirLine->text());
	jplagSimilarity.processDirectory(m_pSimSpecDirLine->text().toStdString());

	QVector<SimilarityResults> comparisons;
	comparisons.reserve(100);    //预分配存储相似度容器大小为100
	jplagSimilarity.outputSpecificDirSim(m_pSimIncludeLine->text(), m_pSimSpecExcludeLine->text(), m_pComboBox->currentText(), comparisons);

	for (const SimilarityResults& res : comparisons)
	{
		QStandardItem* firstNameItem = new QStandardItem(res.firstFileName);
		QStandardItem* secondNameItem = new QStandardItem(res.secondFileName);
		QStandardItem* similarityItem = new QStandardItem(res.similarity);

		m_pSimModel->appendRow({ firstNameItem,secondNameItem ,similarityItem });
		QCoreApplication::processEvents();
	}
}

void KSimWin::handleFileSimClick()
{
	int rowCount = m_pSimModel->rowCount();
	m_pSimModel->removeRows(0, rowCount);

	KJplagSimilarity jplagSimilarity("");

	QVector<SimilarityResults> comparisons;
	jplagSimilarity.outputTwoFileCompareSim(m_pSimFirFileLine->text(), m_pSimSecFileLine->text(), comparisons);

	QStandardItem* firstNameItem = new QStandardItem(comparisons.at(0).firstFileName);
	QStandardItem* secondNameItem = new QStandardItem(comparisons.at(0).secondFileName);
	QStandardItem* similarityItem = new QStandardItem(comparisons.at(0).similarity);

	m_pSimModel->appendRow({ firstNameItem,secondNameItem ,similarityItem });
	QCoreApplication::processEvents();

	comparisons.clear();
}

void KSimWin::handleDirSimClick()
{
	int rowCount = m_pSimModel->rowCount();
	m_pSimModel->removeRows(0, rowCount);

	KJplagSimilarity jplagSimilarity("");

	QVector<SimilarityResults> comparisons;
	jplagSimilarity.outputTwoDirCompareSim(m_pSimFirDirLine->text(), m_pSimSecDirLine->text(), comparisons);

	QStandardItem* firstNameItem = new QStandardItem(comparisons.at(0).firstFileName);
	QStandardItem* secondNameItem = new QStandardItem(comparisons.at(0).secondFileName);
	QStandardItem* similarityItem = new QStandardItem(comparisons.at(0).similarity);

	m_pSimModel->appendRow({ firstNameItem,secondNameItem ,similarityItem });
	QCoreApplication::processEvents();

	comparisons.clear();
}

void KSimWin::handleSimTotalDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pSimTotalDirLine->setText(folderPath);
}

void KSimWin::handleSimSpecDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pSimSpecDirLine->setText(folderPath);
}

void KSimWin::handleSimSpecSubDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), m_pSimSpecDirLine->text());
	if (folderPath.isEmpty())
		return;
	QString subDirPath = folderPath.remove(0, m_pSimSpecDirLine->text().length());
	int firstSlashIndex = subDirPath.indexOf('/');
	subDirPath.replace(firstSlashIndex + 1, subDirPath.indexOf('/', firstSlashIndex + 1) - firstSlashIndex - 1, "*");
	m_pSimIncludeLine->setText(subDirPath);
}

void KSimWin::handleSimFirFileClick()
{
	QString filePath = QFileDialog::getOpenFileName(nullptr, tr("filePathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (filePath.isEmpty())
		return;
	m_pSimFirFileLine->setText(filePath);
}

void KSimWin::handleSimSecFileClick()
{
	QString filePath = QFileDialog::getOpenFileName(nullptr, tr("filePathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (filePath.isEmpty())
		return;
	m_pSimSecFileLine->setText(filePath);
}

void KSimWin::handleSimFirDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pSimFirDirLine->setText(folderPath);
}

void KSimWin::handleSimSecDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::getOpenDirDefaultPath());
	if (folderPath.isEmpty())
		return;
	m_pSimSecDirLine->setText(folderPath);
}

void KSimWin::initSimilarityModel()
{
	m_pSimModel = new QStandardItemModel(0, 3);
	m_pSimModel->setHeaderData(0, Qt::Horizontal, tr("FirstFileName"));
	m_pSimModel->setHeaderData(1, Qt::Horizontal, tr("SecondFileName"));
	m_pSimModel->setHeaderData(2, Qt::Horizontal, tr("Similarity"));
}

void KSimWin::onHandleFileBtnChange()
{
	m_pSimFileBtn->setEnabled(!(m_pSimFirFileLine->text().isEmpty() || m_pSimSecFileLine->text().isEmpty()));
}

void KSimWin::onHandleDirBtnChange()
{
	m_pSimDirBtn->setEnabled(!(m_pSimFirDirLine->text().isEmpty() || m_pSimSecDirLine->text().isEmpty()));
}

QStackedWidget* KSimWin::getStackedWin()
{
	return m_pSimStackWin;
}

void KSimWin::onHandleSpecBtnChange()
{
	m_pSimSpecBtn->setEnabled(!(m_pSimSpecDirLine->text().isEmpty() || m_pSimIncludeLine->text().isEmpty()));
}
