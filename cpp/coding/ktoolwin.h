#ifndef __KXMLREADER_KTOOLWIN_H__
#define __KXMLREADER_KTOOLWIN_H__

#include <QWidget>
#include <QLineEdit>

class KToolWin : public QWidget
{
	Q_OBJECT

public:
	KToolWin(const QString& select, const QString& title, QWidget* parent = nullptr);
	~KToolWin();
signals:
	void configPersonDefaultFilePathEndSignal();
private slots:
	void onHandleOpenDefaultDir();
private:
	KToolWin(const KToolWin& other) = default;
	void operator=(const KToolWin& other) = delete;
	KToolWin(const KToolWin&& other) = delete;
	KToolWin& operator=(KToolWin&& other) = default;

	void initWindow();

	QString m_title;
	QString m_select;

	QLineEdit* m_pLineEdit;
};
#endif
