#include "knextbtn.h"

#include <QMenu>
#include <QContextMenuEvent>

KNextBtn::KNextBtn(const QString& name, QWidget* parent)
    : QPushButton(name, parent)
{
}

KNextBtn::~KNextBtn()
{
}

void KNextBtn::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    for (const QString& dir : m_dirPath)
    {
        QAction* action = menu.addAction(dir);
        (void)connect(action, &QAction::triggered, this, &KNextBtn::onRightClick);
    }
    menu.exec(event->globalPos());
}

void KNextBtn::onRightClick()
{
    QAction* clickedAction = qobject_cast<QAction*>(sender());
    if (clickedAction)
    {
        emit sendMenuActionSignal(clickedAction->text());
    }
}

void KNextBtn::converDirToNextBtn(const QVector<QString>& dirPath)
{
    m_dirPath = dirPath;
}