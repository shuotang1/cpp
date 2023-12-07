#include "kdatabase.h"
#include "krallfile.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QTextStream>

KDataBase::KDataBase()
{
	initDataBase("cpp.db");
}

KDataBase::~KDataBase()
{
	if (m_db.isOpen())
		m_db.close();
}

bool KDataBase::initDataBase(const QString& dbname)
{
	if (m_db.tables().empty())
	{
		m_db.setDatabaseName(dbname);
		bool ok = m_db.open();
		if (!ok)	
			return false;
		ok = createTable("CppCheckType");
		if (!ok)
			return false;
		ok = createConfigTable("config");
		if (!ok)
			return false;
		ok = createScoreTable("scorecomment");
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::createTable(const QString& tabname)
{
	bool isExistTable = m_db.tables().contains(tabname);
	if (!isExistTable)
	{
		QSqlQuery query;
		//" %1 %2 %3" .arg("xxx").arg("yyyy")
		QString sql = QString("CREATE TABLE IF NOT EXISTS %1(id text not null"
			",severity text"
			",msg text"
			",verbose text"
			",cwe text"
			",file text,line text,column text,info text,symbol text)").arg(tabname);
		bool ok = query.exec(sql);
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::createConfigTable(const QString& configTabName)
{
	bool isExistTable = m_db.tables().contains(configTabName);
	if (!isExistTable)
	{
		QSqlQuery query;
		QString sql = QString("CREATE TABLE IF NOT EXISTS %1(rootUrl text, token text)").arg(configTabName);
		bool ok = query.exec(sql);
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::createScoreTable(const QString& tabname)
{
	bool isExistTable = m_db.tables().contains(tabname);
	if (!isExistTable)
	{
		QSqlQuery query;
		QString sql = QString("CREATE TABLE IF NOT EXISTS %1(fileName text, scoreValue text, comment text)").arg(tabname);
		bool ok = query.exec(sql);
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::inputRecord(const QVector<QString>& cppcheckTable)
{
	QSqlQuery query;
	QString sql = "INSERT INTO CppCheckType(id,severity,msg,verbose,cwe,file,line,column,info,symbol) VALUES(?,?,?,?,?,?,?,?,?,?);";
	query.prepare(sql);

	int index = 0;
	for (const auto& str : cppcheckTable)
	{
		query.bindValue(index, str);
		index++;
	}
	bool ok = query.exec();
	if (!ok)
		return false;
	return true;
}

bool KDataBase::insertConfig(const QVector<QString>& configVec)
{
	QSqlQuery query;
	QString sql = "INSERT INTO config(rootUrl,token) VALUES(?,?);";
	query.prepare(sql);
	query.bindValue(0, configVec[0]);
	query.bindValue(1, configVec[1]);
	bool ok = query.exec();
	if (!ok)
		return false;
	return true;
}

#include <qDebug>
#include <QSqlError>
bool KDataBase::insertScore(const QVector<QString>& scoreVec)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM scorecomment WHERE fileName = ?");
	query.addBindValue(scoreVec[0]);

	//--TODO 未知错误
	if (query.exec() && query.next())
	{
		query.prepare("UPDATE scorecomment SET scoreValue = ?, comment = ? WHERE fileName = ?");
		query.addBindValue(scoreVec[1]);
		query.addBindValue(scoreVec[2]);
		query.addBindValue(scoreVec[0]);
		if (!query.exec())
		{
			qDebug() << query.lastError().text();
			return false;
		}
	}
	else
	{
		QString sql = "INSERT INTO scorecomment(fileName,scoreValue,comment) VALUES(?,?,?);";
		query.prepare(sql);
		query.bindValue(0, scoreVec[0]);
		query.bindValue(1, scoreVec[1]);
		query.bindValue(2, scoreVec[2]);
		if (!query.exec())
		{
			qDebug() << query.lastError().text();
			return false;
		}
	}
	return true;
}

bool KDataBase::selectScore(const QString& fileName, QVector<QString>& output)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM scorecomment WHERE fileName = ?");
	query.addBindValue(fileName);
	if (query.exec() && query.next())
	{
		output.push_back(query.value(1).toString());
		output.push_back(query.value(2).toString());
		return true;
	}
	return false;
}

bool KDataBase::clearRecord()
{
	QSqlQuery query;
	query.prepare("DELETE FROM CppCheckType");
	if (!query.exec())
		return false;
	return true;
}

bool KDataBase::clearConfig()
{
	QSqlQuery query;
	query.prepare("DELETE FROM config");
	if (!query.exec())
		return false;
	return true;
}

bool KDataBase::clearScore()
{
	QSqlQuery query;
	query.prepare("DELETE FROM scorecomment");
	if (!query.exec())
		return false;
	return true;
}

bool KDataBase::saveCSV(const QString& type, const QString& filePath)
{
	QSqlQuery query;
	if (type == "cppcheck")
		query.prepare("SELECT * FROM CppCheckType;");
	else if (type == "score")
		query.prepare("SELECT * FROM scorecomment;");
	if (!query.exec())
		return false;

	KRallFile file(filePath, "write");
	QTextStream stream(&(file.getFile()));

	// 写入表头
	QStringList headerRow;
	for (int i = 0; i < query.record().count(); ++i)
	{
		headerRow << query.record().fieldName(i);
	}
	stream << headerRow.join(",") << "\n";

	// 写入数据
	while (query.next())
	{
		QStringList rowData;
		for (int i = 0; i < query.record().count(); ++i)
		{
			QString value = query.value(i).toString();
			if (value.contains(",") || value.contains("\"") || value.contains("\n"))
			{
				value.replace("\"", "\"\"");       //将双引号替换为两个双引号
				value = "\"" + value + "\"";       //csv将','作为分隔符，因此需要在含有','的字符中，将其变为一个整体字符串
			}
			value.replace("\n", "\r\n");           // 将换行符\n替换为\r\n  
			rowData << value;
		}
		stream << rowData.join(",") << "\n";
	}
	return true;
}

bool KDataBase::selectConfig(QVector<QString>& configVec)
{
	QSqlQuery query;
	QString sql = "SELECT * FROM config";
	query.prepare(sql);
	if (!query.exec())
		return false;
	int columnLen = query.record().count();
	while (query.next())
	{
		for (int i = 0; i < columnLen; i++)
		{
			configVec.push_back(query.value(i).toString());
		}
	}
	return true;
}