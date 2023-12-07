#ifndef __KXMLREADER_KSCORINGWIN_H__
#define __KXMLREADER_KSCORINGWIN_H__

#include "ksavewindow.h"

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>

class KScoringWin : public QWidget
{
	Q_OBJECT

public:
	KScoringWin(QWidget *parent);
	~KScoringWin();

	void updateScoreWin(const QString& score, const QString& comment);
	void saveScoreToDb(const QString& fileName);
private slots:
	void onHandleExportClick();
	void onHandleClearClick();
private:
	void initWin();

	QLineEdit* m_pLineEdit;
	QPlainTextEdit* m_pTextEdit;
	std::unique_ptr<KSaveWindow> m_pSaveWin;
};
#endif
