#ifndef __KXMLREADER_KCPPCHECKXML_H__
#define __KXMLREADER_KCPPCHECKXML_H__ 

#include<Qvector>

struct ErrorInfo
{
    QString id;                          //����warnning��error��style��performance....����id
    QString severity;                    //warning��error��style....
    QString msg;                         //����ļ�Ҫ����
    QString verbose;                     //��ϸ�Ĵ�������
    QString cwe = "NULL";                //����������Common Weakness Enumeration��CWE������
    QString file;                        //���������ļ�·��
    int line;                            //���������к�
    int column;                          //���������к�
    QString info = "NULL";               //�йش���Ķ�����Ϣ
    QString symbol = "none";             //�������صķ��Ż������
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

