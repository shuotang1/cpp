#ifndef __KXMLREADER_KCOMPILE_H__
#define __KXMLREADER_KCOMPILE_H__

#include <QThread>

class KCompile : public QThread
{
public:
	KCompile(const QString& path);
	~KCompile();

	void run()override;      //将命令行编译程序放入一个单门的线程中执行 
	void getExeFileName(QVector<QString>& fileName);

	QStringList getDirLogs(const QString& path);    //将编译目录日志保存至QStringList中
private:
	KCompile(const KCompile& other) = default;
	void operator=(const KCompile& other) = delete;
	KCompile(const KCompile&& other) = delete;
	KCompile& operator=(KCompile&& other) = default;

	QString m_path; 
};
#endif
