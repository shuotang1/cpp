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
	void handleSearchClick();                       //��ʾ��ѯ����
	void handleWebBackgroundChange(bool isChecked); //�ı�webҳ�汳����ɫ
	void handleWebFontSizeChange(int value);        //�ı�webҳ�������С
	void bindDirTreeAndMenu(const QPoint& pos);     //��Ŀ¼�����Ҽ��˵�
	void handleOpenDirTreeFileAction();             //��Ŀ¼��ѡ���ļ�
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
	QString m_filePath;         //webҳ��������ʾ���ļ�·��
};

#endif
