#ifndef __KXMLREADER_KNEXTBTN_H__
#define __KXMLREADER_KNEXTBTN_H__

#include <QPushButton>
#include <QVector>
#include <QObject>

class KNextBtn : public QPushButton
{
	Q_OBJECT

public:
	KNextBtn(const QString& name, QWidget* parent = nullptr);
	~KNextBtn();

	void converDirToNextBtn(const QVector<QString>& dirPath);
signals:
	void sendMenuActionSignal(const QString& dir);
private slots:
	void onRightClick();
private:
	void contextMenuEvent(QContextMenuEvent* event) override;

	QVector<QString> m_dirPath;
};
#endif
