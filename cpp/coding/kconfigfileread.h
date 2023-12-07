#ifndef __KXMLREADER_KCONFIGREADER_H__
#define __KXMLREADER_KCONFIGREADER_H__

#include "kglobaldata.h"
#include "krallfile.h"

#include <QVector>
#include <QMap>
#include <QXmlStreamReader>

QMap<QString, QVector<QString>> ReadConfigFile(const QString& filePath)
{
	QMap<QString, QVector<QString>> maps;
	KRallFile file(filePath, "read");
	QXmlStreamReader xmlReader(&file.getFile());

	while (!xmlReader.atEnd() && !xmlReader.hasError())
	{
		QVector<QString> vec;
		vec.reserve(4);
		QXmlStreamReader::TokenType token = xmlReader.readNext();
		if (token == QXmlStreamReader::StartElement)
		{
			QString elementName = xmlReader.name().toString();
			if (elementName == "Widget")
			{
				vec.push_back(xmlReader.attributes().value("blackIcon").toString());
				vec.push_back(xmlReader.attributes().value("lightIcon").toString());
				vec.push_back(xmlReader.attributes().value("text").toString());
				maps[xmlReader.attributes().value("widget").toString()] = vec;
			}
		}
	}
	return maps;
}

#endif
