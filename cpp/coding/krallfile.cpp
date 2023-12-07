#include "krallfile.h"

#include <QMessageBox>

KRallFile::KRallFile(const QString& filePath, const QString& label)
	: m_file(filePath)
	, m_label(label)
{
	openFile();
}

void KRallFile::openFile()
{
	if (m_label == "write")
	{
		if (!m_file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(Q_NULLPTR, QStringLiteral("openFailed"), QObject::tr("openWriteFileFailedTr"), QMessageBox::Ok);
			return;
		}
	}
	else if (m_label == "read")
	{
		if (!m_file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(Q_NULLPTR, QStringLiteral("openFailed"), QObject::tr("openReadFileFailedTr"), QMessageBox::Ok);
			return;
		}
	}
	else if (m_label == "clearBeforeWrite")
	{
		if (!m_file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		{
			QMessageBox::warning(Q_NULLPTR, QStringLiteral("openFailed"), QObject::tr("openClearBeforeWriteFileFailedTr"), QMessageBox::Ok);
			return;
		}
	}
}

KRallFile::~KRallFile()
{
	m_file.close();
}

QFile& KRallFile::getFile()
{
	return m_file;
}
