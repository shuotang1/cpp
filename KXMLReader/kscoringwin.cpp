#include "kscoringwin.h"
#include "kdatabase.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>
#include <QMessageBox>

KScoringWin::KScoringWin(QWidget *parent)
	: QWidget(parent)
	, m_pLineEdit(nullptr)
	, m_pTextEdit(nullptr)
	, m_pSaveWin(nullptr)
{
	initWin();
}

KScoringWin::~KScoringWin()
{
}

void KScoringWin::initWin()
{
	setStyleSheet("background-color: #ECECEC;");
	QFont font;
	font.setPointSize(12);
	QLabel* scoreLabel = new QLabel(tr("scoreLabelTr") + ":", this);
	QLabel* commentsLabel = new QLabel(tr("commentsLabelTr") + ":", this);
	scoreLabel->setFont(font);
	commentsLabel->setFont(font);
	m_pLineEdit = new QLineEdit(this);
	m_pLineEdit->setPlaceholderText("0");
	QRegExp regex("[0-9.]*");
	QValidator* validator = new QRegExpValidator(regex, m_pLineEdit);
	m_pLineEdit->setValidator(validator);
	m_pTextEdit = new QPlainTextEdit(this);
	m_pTextEdit->setPlaceholderText(tr("textEditHolderTextTr"));
	QPushButton* exportBtn = new QPushButton(tr("exportBtnTr"), this);
	QPushButton* clearBtn = new QPushButton(tr("clearBtnTr"), this);
	exportBtn->setFont(font);
	clearBtn->setFont(font);

	QHBoxLayout* m_pHLineLay = new QHBoxLayout;
	m_pHLineLay->addWidget(scoreLabel);
	m_pHLineLay->addWidget(m_pLineEdit);
	QHBoxLayout* m_pHTextLay = new QHBoxLayout;
	m_pHTextLay->addWidget(commentsLabel);
	m_pHTextLay->addWidget(m_pTextEdit);
	QHBoxLayout* m_pHBtntLay = new QHBoxLayout;
	m_pHBtntLay->addWidget(clearBtn);
	m_pHBtntLay->addWidget(exportBtn);

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->addLayout(m_pHLineLay);
	m_pVMainLay->addLayout(m_pHTextLay);
	m_pVMainLay->addLayout(m_pHBtntLay);

	(void)connect(exportBtn, &QPushButton::clicked, this, &KScoringWin::onHandleExportClick);
	(void)connect(clearBtn, &QPushButton::clicked, this, &KScoringWin::onHandleClearClick);
}

void KScoringWin::onHandleExportClick()
{
	m_pSaveWin = std::make_unique<KSaveWindow>();
	m_pSaveWin->setSaveTable(1);    //0:cppcheck表   1:分数表
	m_pSaveWin->show();
}

void KScoringWin::updateScoreWin(const QString& score, const QString& comment)
{
	m_pLineEdit->setText(score);
	m_pTextEdit->setPlainText(comment);
}

void KScoringWin::onHandleClearClick()
{
	QMessageBox::StandardButton reply = QMessageBox::question(this, tr("okTr"),
		tr("clearScoreTr") + "?", QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		KDataBase db;
		if (!db.clearScore())
		{
			QMessageBox::warning(Q_NULLPTR, "clearFailed", tr("clearScoreTableFailed"), QMessageBox::Ok);
			return;
		}
		QMessageBox::warning(Q_NULLPTR, "clearSuccess", tr("clearScoreSuccessTr"), QMessageBox::Ok);
	}
}

void KScoringWin::saveScoreToDb(const QString& fileName)
{
	KDataBase db;
	QVector<QString> scoreVec = { fileName,m_pLineEdit->text(),m_pTextEdit->toPlainText() };
	if (!db.insertScore(scoreVec))
		QMessageBox::warning(Q_NULLPTR, "insertFailed", tr("insertScoreTableFailed"), QMessageBox::Ok);
}