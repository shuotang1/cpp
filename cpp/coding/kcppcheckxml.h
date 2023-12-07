#ifndef __KXMLREADER_KCPPCHECKXML_H__
#define __KXMLREADER_KCPPCHECKXML_H__ 

#include<Qvector>

struct ErrorInfo
{
    QString id;                          //描述warnning，error，style，performance....内容id
    QString severity;                    //warning，error，style....
    QString msg;                         //错误的简要描述
    QString verbose;                     //详细的错误描述
    QString cwe = "NULL";                //错误所属的Common Weakness Enumeration（CWE）分类
    QString file;                        //错误所在文件路径
    int line;                            //错误所在行号
    int column;                          //错误所在列号
    QString info = "NULL";               //有关错误的额外信息
    QString symbol = "none";             //与错误相关的符号或变量名
};

class KCppCheckXML
{
public:
	KCppCheckXML();
	~KCppCheckXML();

    void outputCppcheckRunResult(const QString& dirPath, QVector<ErrorInfo>& output);
    void outputCppCheckSpecRunResult(const QString& rootPath, const QString& subPath, QVector<ErrorInfo>& output);
private:
    KCppCheckXML(const KCppCheckXML& other) = default;
    void operator=(const KCppCheckXML& other) = delete;
    KCppCheckXML(const KCppCheckXML&& other) = delete;
    KCppCheckXML& operator=(KCppCheckXML&& other) = default;

    void outputRunCppcheckToCsv(const QString& command);
    void readCppcheckCsv(const QString& csvFilePath, QVector<ErrorInfo>& output);
};
#endif

