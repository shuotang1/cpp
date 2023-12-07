#ifndef __KXMLREADER_KCUSTOMITEMDELEGATE_H__
#define __KXMLREADER_KCUSTOMITEMDELEGATE_H__

#include "kcppcheckxml.h"

#include <QObject>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QVector>
#include <QStandardItemModel>

class KCustomItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	KCustomItemDelegate(QObject* parent = nullptr);
	~KCustomItemDelegate();

	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

	void executeOptToBackMemory(const QVector<ErrorInfo>& errorList);
	void clearOptToBackMemory();
private:
	KCustomItemDelegate(const KCustomItemDelegate& other) = default;
	void operator=(const KCustomItemDelegate& other) = delete;
	KCustomItemDelegate(const KCustomItemDelegate&& other) = delete;
	KCustomItemDelegate& operator=(KCustomItemDelegate&& other) = default;

	QVector<QPair<const QString, bool>> m_optToBackVec;
};
#endif
