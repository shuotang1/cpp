#ifndef __KXMLREADER_KFILEWIN_H__
#define __KXMLREADER_KFILEWIN_H__

#include "knextbtn.h"
#include "kscoringwin.h"
#include "ksearchwin.h"

#include <QWidget>
#include <QLineEdit>
#include <QDirModel>
#include <QTreeView>
#include <QVector>
#include <QTextEdit>
#include <QLabel>
#include <QWebEngineView>
#include <QCheckBox>
#include <QSpinBox>
#include <QMenu>

class KFileWin : public QWidget
{
	Q_OBJECT

public:
	KFileWin(QWidget* parent = nullptr);
	~KFileWin();
signals:
	void compileSignal(const QString& dirPath);
private slots:
	void handleRootDirClick();
	void handleSubDirClick();
	void handleNextClick();
	void handlePreClick();
	void handleNextBtnSingal(const QString& dir);
	void handleDirTreeDoubleClick(const QModelIndex& index);
	void handleCompileClick();
	void handleSearchClick();                       //显示查询窗口
	void handleWebBackgroundChange(bool isChecked); //改变web页面背景颜色
	void handleWebFontSizeChange(int value);        //改变web页面字体大小
	void bindDirTreeAndMenu(const QPoint& pos);     //绑定目录树与右键菜单
	void handleOpenDirTreeFileAction();             //打开目录树选中文件
private:
	void initWindow();
	void updateScoringWin();
	void showDirTree();
	void showFileInWeb(const QString& filePath);

	QLineEdit* m_pRootDirLine;
	QLineEdit* m_pSubDirLine; 
	KNextBtn* m_pPreBtn;
	KNextBtn* m_pNextBtn;
	QPushButton* m_pCompileBtn;
	QPushButton* m_pSearchBtn;
	QLabel* m_pCurrentDirLabel;
	QCheckBox* m_pWebBackgroundBox;
	QSpinBox* m_pWebFontSize;
	QMenu* m_pOpenDirTreeFileMenu;

	QDirModel* m_pDirModel;
	QTreeView* m_pTreeView;
	QWebEngineView* m_pWeb;
	KScoringWin* m_pScoringWin;
	std::unique_ptr<KSearchWin> m_pSearchWin;

	QVector<QString> m_file;
	QVector<QString> m_dirPath;
	int m_index;
	QString m_filePath;         //web页面正在显示的文件路径
};

#endif
