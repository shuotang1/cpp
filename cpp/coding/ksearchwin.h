#ifndef __KXMLREADER_KSEARCHWIN_H__
#define __KXMLREADER_KSEARCHWIN_H__

#include <QWidget>
#include <QWebEngineView>

class KSearchWin : public QWidget
{
	Q_OBJECT

public:
	KSearchWin(QWebEngineView* webView, QWidget* parent = nullptr);
	~KSearchWin();
private:
	void initWin();
	void highlightMatches(const QString& searchTerm);  //将web中匹配的用橙色背景标记
	
	QWebEngineView* m_pWebView;
};
#endif
