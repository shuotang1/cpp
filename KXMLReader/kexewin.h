#ifndef __KXMLREADER_KEXEWIN_H__
#define __KXMLREADER_KEXEWIN_H__

#include <QWidget>
#include<QLineEdit>
#include<QList>
#include<QHBoxLayout>

class KExeWin : public QWidget
{
	Q_OBJECT

public:
	KExeWin(const QString& fileName, QWidget* parent = nullptr);
	~KExeWin();

	void initWindow();
private slots:
	void onHandleAddInputLineClick();
	void onHandleRunClick();
private:
	KExeWin(const KExeWin& other) = default;
	void operator=(const KExeWin& other) = delete;
	KExeWin(const KExeWin&& other) = delete;
	KExeWin& operator=(KExeWin&& other) = default;

	QString m_fileName;

	QList<QLineEdit*> m_lineList;       //存储输入参数列表

	QVBoxLayout* m_PVBoxLay;
	QLineEdit* m_pLineEdit;             //存储输出参数
};
#endif
