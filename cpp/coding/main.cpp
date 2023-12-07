#include "kmainwindow.h"
#include "kglobaldata.h"
#include "kconfigfileread.h"
#include "kcppcheckwin.h"
#include "ksimwin.h"
#include "kcompilewin.h"
#include "kgitwin.h"
#include "kfilewin.h"

#include <QtWidgets/QApplication>
#include <QTranslator>
//#include <vld.h>
//#pragma comment(lib, "vld.lib")

int main(int argc, char *argv[])
{
    std::vector<int >v;
    QApplication a(argc, argv);   

    QTranslator trans;
    trans.load(KGlobalDataNameSpace::getFatherDirName() + "\\env\\lang\\kxmlreader_zh.qm");
    a.installTranslator(&trans);

    KMainWindow w;

    QMap<QString, QVector<QString>> maps = ReadConfigFile(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_configFileDir);

    KCppCheckWin* m_pCppCheckWin = new KCppCheckWin;
    w.addNewWidget(maps["KCppCheckWin"][0], maps["KCppCheckWin"][1], maps["KCppCheckWin"][2], m_pCppCheckWin);

    KSimWin* m_pSimWin = new KSimWin;
    w.addNewWidget(maps["KSimWin"][0], maps["KSimWin"][1], maps["KSimWin"][2], m_pSimWin);

    KCompileWin* m_pCompileWin = new KCompileWin;
    w.addNewWidget(maps["KCompileWin"][0], maps["KCompileWin"][1], maps["KCompileWin"][2], m_pCompileWin);

    KGitWin* m_pGitWin = new KGitWin;
    w.addNewWidget(maps["KGitWin"][0], maps["KGitWin"][1], maps["KGitWin"][2], m_pGitWin);

    KFileWin* m_pFileWin = new KFileWin;
    w.addNewWidget(maps["KFileWin"][0], maps["KFileWin"][1], maps["KFileWin"][2], m_pFileWin);

    w.setDefaultSelected(0);
    w.show();
    return a.exec();
}
