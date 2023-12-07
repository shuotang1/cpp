#include "kcppcheckxml.h"
#include "kglobaldata.h"
#include "krallfile.h"

#include <QProcess>  
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QDirIterator>

KCppCheckXML::KCppCheckXML()
{
}

KCppCheckXML::~KCppCheckXML()
{

}

void KCppCheckXML::outputRunCppcheckToCsv(const QString& command)
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);  // 将标准输出与标准错误合并
    process.start(command);
    process.waitForFinished();

    QByteArray outputData = process.readAll();
    QTextCodec* codec = QTextCodec::codecForLocale();         //获取当前系统本地编码对象
    if (codec == nullptr)
        return;
    QString output = codec->toUnicode(outputData);            //将字节流解码为Unicode字符

    //保存至XML文件
    KRallFile outputFile("output.xml", "clearBeforeWrite");
    QTextStream stream(&outputFile.getFile());
    stream.setGenerateByteOrderMark(true);
    stream.setCodec("UTF-8");
    stream << output; 
}

void KCppCheckXML::readCppcheckCsv(const QString& csvFilePath, QVector<ErrorInfo>& output)
{
    KRallFile file(csvFilePath, "read");
    QXmlStreamReader xmlReader(&file.getFile());

    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        // 跳过StartDocument标记，继续读取下一个标记
        if (token == QXmlStreamReader::StartDocument)
            continue;

        if (token == QXmlStreamReader::Invalid)
            break;

        if (token == QXmlStreamReader::StartElement)
        {
            QString elementName = xmlReader.name().toString();
            if (elementName == "error")
            {
                ErrorInfo error;
 
                QXmlStreamAttributes attributes = xmlReader.attributes();
                error.id = attributes.value("id").toString();
                error.severity = attributes.value("severity").toString();
                error.msg = attributes.value("msg").toString();
                error.verbose = attributes.value("verbose").toString();
                error.cwe = attributes.value("cwe").toString();

                // 解析error元素的子元素  
                while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "error"))
                {
                    xmlReader.readNext();

                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
                    {
                        if (xmlReader.name() == "location")
                        {
                            error.file = xmlReader.attributes().value("file").toString();
                            error.line = xmlReader.attributes().value("line").toInt();
                            error.column = xmlReader.attributes().value("column").toInt();
                            error.info = xmlReader.attributes().value("info").toString();
                        }
                        else if (xmlReader.name() == "symbol")
                        {
                            xmlReader.readNext();
                            error.symbol = xmlReader.text().toString();
                        }
                    }
                }
                output.push_back(error);
            }
        }
    }
}

void KCppCheckXML::outputCppcheckRunResult(const QString& dirPath, QVector<ErrorInfo>& output)
{
    QString command = KGlobalDataNameSpace::getFatherDirName() + "\\env\\CPPcheck"
        + "\\cppcheck --platform=win64 --enable=warning,style,performance,portability  --verbose --xml " + dirPath;
    outputRunCppcheckToCsv(command);
    readCppcheckCsv("output.xml", output);
}

void KCppCheckXML::outputCppCheckSpecRunResult(const QString& rootPath, const QString& subPath, QVector<ErrorInfo>& output)
{
    QVector<QString> dirVec;
    dirVec.reserve(100);
    QDirIterator it(rootPath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString absoluteFilePath = it.next();
        QString lastDirName = absoluteFilePath.mid(rootPath.length());

        int slashIndex1 = lastDirName.indexOf('/');
        int slashIndex2 = lastDirName.indexOf('/', slashIndex1 + 1);
        if (slashIndex1 != -1 && slashIndex2 != -1)
        {
            QString subPath1 = lastDirName.mid(slashIndex2 + 1);
            QString subPath2 = subPath.mid(subPath.indexOf('/', subPath.indexOf('/') + 1) + 1);

            if (subPath1 == subPath2)
                dirVec.push_back(absoluteFilePath);
        }
    }
    QString command = KGlobalDataNameSpace::getFatherDirName() + "\\CPPcheck"
        + "\\cppcheck --platform=win64 --enable=warning,style,performance,portability  --verbose --xml ";
    for (const QString& dir : dirVec)
    {
        command = command + dir + " ";
    }
    outputRunCppcheckToCsv(command);
    readCppcheckCsv("output.xml", output);
}
