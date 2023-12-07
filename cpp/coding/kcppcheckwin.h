#ifndef __KXMLREADER_KCPPCHECKWIN_H__
#define __KXMLREADER_KCPPCHECKWIN_H__

#include "kcustomitemdelegate.h"
#include "kerrorshowmainwin.h"

#include <QWidget>
#include <QLineEdit>
#include <QTableView>
#include <QMenu>
#include <QVector>
#include <QStackedWidget>

class KCppCheckWin : public QWidget
{
	Q_OBJECT

public:
	KCppCheckWin(QWidget* parent = nullptr);
	~KCppCheckWin();

	QStackedWidget* getStackedWin();
private slots:
	void handleCheckDirClick();
	void handleCheckClick();
	void handleSpecCheckClick();
	void handleRootDirClick();
	void handleSubDirClick();
private:
	KCppCheckWin(const KCppCheckWin& other) = default;
	void operator=(const KCppCheckWin& other) = delete;
	KCppCheckWin(const KCppCheckWin&& other) = delete;
	KCppCheckWin& operator=(KCppCheckWin&& other) = default;

	void initCheckModel();
	void initWin();

	void addNewRowToModel(const QVector<ErrorInfo>& errorList);
	void addNewRowToDb(const QVector<ErrorInfo>& errorList);

	QLineEdit* m_pCheckDirectoryLine;
	QLineEdit* m_pSpecRootDirLine;
	QLineEdit* m_pSpecSubDirLine;
	QTableView* m_pTableView;
	QStandardItemModel* m_pModel;
	KCustomItemDelegate* m_pDelegate;
	QMenu* m_pTableViewMenu;
	QStackedWidget* m_pStackedWin;

	std::unique_ptr<KErrorShowMainWin> m_pErrorShowMainWin;
};
#endif
