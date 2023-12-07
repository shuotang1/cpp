#ifndef __KXMLREADER_KERRORSHOWMAINWIN_H__
#define __KXMLREADER_KERRORSHOWMAINWIN_H__

#include <QWidget>
#include <QPlainTextEdit>

class KErrorShowMainWin : public QWidget
{
	Q_OBJECT

public:
	KErrorShowMainWin(const QString& fileName, int lineNumber, int fileToTableLine, QWidget* parent = nullptr);
	~KErrorShowMainWin();
private:
	KErrorShowMainWin(const KErrorShowMainWin& other) = default;
	void operator=(const KErrorShowMainWin& other) = delete;
	KErrorShowMainWin(const KErrorShowMainWin&& other) = delete;
	KErrorShowMainWin& operator=(KErrorShowMainWin&& other) = default;

	void initWindow();

	QPlainTextEdit* m_pPlainTextEdit;

	QString m_fileName;
	int m_line;
	int m_fileToTableLine;
};
#endif
