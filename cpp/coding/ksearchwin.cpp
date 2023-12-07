#include "ksearchwin.h"

#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>

KSearchWin::KSearchWin(QWebEngineView* webView, QWidget* parent)
	: QWidget(parent)
	, m_pWebView(webView)
{
	initWin();
}

KSearchWin::~KSearchWin()
{
}

void KSearchWin::initWin()
{
	setWindowFlags(Qt::FramelessWindowHint);  // Òþ²Ø´°¿Ú±êÌâÀ¸  

	QLineEdit* m_pLineEdit = new QLineEdit(this);
	m_pLineEdit->setPlaceholderText(tr("searchLineEditHolderTextTr") + "...");
	QPushButton* nextBtn = new QPushButton(QIcon(":/icons/next.png"), "", this);
	QPushButton* preBtn = new QPushButton(QIcon(":/icons/pre.png"), "", this);
	QPushButton* exitBtn = new QPushButton(QIcon(":/icons/close.png"), "", this);
	nextBtn->setEnabled(false);
	preBtn->setEnabled(false);
	
	QHBoxLayout* m_pHMainLay = new QHBoxLayout(this);
	m_pHMainLay->addWidget(m_pLineEdit);
	m_pHMainLay->addWidget(nextBtn);
	m_pHMainLay->addWidget(preBtn);
	m_pHMainLay->addWidget(exitBtn);

	(void)connect(m_pLineEdit, &QLineEdit::textChanged, this, [=]() {
		QString searchTerm = m_pLineEdit->text();
		nextBtn->setEnabled(!searchTerm.isEmpty());
		preBtn->setEnabled(!searchTerm.isEmpty());
		QString script = QString("window.find('%1');").arg(searchTerm);
		m_pWebView->page()->runJavaScript(script);
		highlightMatches(searchTerm);
	});

	(void)connect(nextBtn, &QPushButton::clicked, this, [=]() {
		QString script = QString("window.find('%1', false, false, true);").arg(m_pLineEdit->text());
		m_pWebView->page()->runJavaScript(script);
	});

	(void)connect(preBtn, &QPushButton::clicked, this, [=]() {
		QString script = QString("window.find('%1', false, true, true);").arg(m_pLineEdit->text());
		m_pWebView->page()->runJavaScript(script);
	});

	(void)connect(exitBtn, &QPushButton::clicked, this, [&]() {
		this->close();
	});
}

void KSearchWin::highlightMatches(const QString& searchTerm)
{
	
}
