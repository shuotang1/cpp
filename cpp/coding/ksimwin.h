#ifndef __KXMLREADER_KSIMWIN_H__
#define __KXMLREADER_KSIMWIN_H__

#include <QWidget>
#include <QTableView>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QPushButton>
#include <QStackedWidget>
#include <QComboBox>

class KSimWin : public QWidget
{
	Q_OBJECT

public:
	KSimWin(QWidget* parent = nullptr);
	~KSimWin();

	QStackedWidget* getStackedWin();
private slots:
	void handleSimTotalDirClick();
	void handleSimSpecDirClick();
	void handleSimSpecSubDirClick();

	void handleSimFirFileClick();
	void handleSimSecFileClick();
	void handleSimFirDirClick();
	void handleSimSecDirClick();

	void handleSimTotalClick();
	void handleSimSpecClick();
	void handleFileSimClick();
	void handleDirSimClick();

	void onHandleFileBtnChange();
	void onHandleDirBtnChange();
	void onHandleSpecBtnChange();
private:
	KSimWin(const KSimWin& other) = default;
	void operator=(const KSimWin& other) = delete;
	KSimWin(const KSimWin&& other) = delete;
	KSimWin& operator=(KSimWin&& other) = default;

	void initSimilarityModel();
	void initWin();

	QLineEdit* m_pSimTotalDirLine;
	QLineEdit* m_pSimSpecDirLine;
	QLineEdit* m_pSimIncludeLine;            //包含目录文本编辑行
	QLineEdit* m_pSimSpecExcludeLine;
	QLineEdit* m_pSimFirFileLine;
	QLineEdit* m_pSimSecFileLine;
	QLineEdit* m_pSimFirDirLine;
	QLineEdit* m_pSimSecDirLine;

	QStackedWidget* m_pSimStackWin;

	QPushButton* m_pSimFileBtn;
	QPushButton* m_pSimSpecBtn;
	QPushButton* m_pSimDirBtn;

	QTableView* m_pSimTableView;          //相似度界面:QTableView
	QStandardItemModel* m_pSimModel;

	QComboBox* m_pComboBox;
};
#endif