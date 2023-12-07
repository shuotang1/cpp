#ifndef __KXMLREADER_KRALLFILE_H__
#define __KXMLREADER_KRALLFILE_H__

#include <QFile>

class KRallFile 
{
public:
	KRallFile(const QString& filePath, const QString& label);
	~KRallFile();

	QFile& getFile();
private:
	KRallFile(const KRallFile& other) = default;
	void operator=(const KRallFile& other) = delete;
	KRallFile(const KRallFile&& other) = delete;
	KRallFile& operator=(KRallFile&& other) = default;

	void openFile();

	QFile m_file;
	QString m_label;
};
#endif
