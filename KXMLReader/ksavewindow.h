#ifndef __KFILELISTENER_KSAVEWINDOW_H__
#define __KFILELISTENER_KSAVEWINDOW_H__

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>

class KSaveWindow : public QWidget
{
	Q_OBJECT

public:
	KSaveWindow(QWidget* parent = nullptr);
	~KSaveWindow();

	void initWindow();
	void setSaveTable(int index); 
private slots:
	void onHandleDirectoryOpen();       //获取存储文件目录
	void onHandleSaveClicked();         //处理SVG文件保存
	void onHandleCancleClicked();       //Cancle取消按钮点击
private:
	KSaveWindow(const KSaveWindow& other) = default;
	void operator=(const KSaveWindow& other) = delete;
	KSaveWindow(const KSaveWindow&& other) = delete;
	KSaveWindow& operator=(KSaveWindow&& other) = default;

	QLineEdit* m_pDirectoryLine;
	QComboBox* m_pReportFormatBox;
};
#endif
