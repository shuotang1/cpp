#ifndef __KFILELISTENER_KGLOBALDATA_H__
#define __KFILELISTENER_KGLOBALDATA_H__

#include <QStandardPaths>
#include <QString>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QSettings>

namespace KGlobalDataNameSpace
{
	inline QString m_fatherDirName;                      //��ǰĿ¼����
	inline QString m_compileDir;                         //����Ŀ¼    
	inline QString m_isSelected = "1111111111";          //�洢��ͼ�б�ѡ�еĲ˵���    
	inline QString m_rootUrl;                            //root_url    
	inline QString m_token;                              //token    
	inline QString m_personFilePath;                     //�ֶ�����Ĭ���ļ�·�� 

	inline const QString m_configFileDir = "\\env\\config\\config.xml";
	inline const QString m_highlightExeDir = "\\env\\bat\\dist\\highlight.exe";
	inline const QString m_wpsgogsgitExeDir = "\\env\\bat\\dist\\wpsgogsgit.exe";
	inline const QString m_jplagExeDir = "\\env\\jplag\\jplag.exe";
	inline const QString m_htmlFileDir = "\\env\\bat\\dist\\res.html";
	inline const QString m_jplagOutputDir = "\\env\\jplag\\output.log";
	inline const QString m_repoTempTopDir = "\\env\\bat\\dist\\venv";         //��Ŵ����ֿ�ʱ������ʱ�ļ���Temp�ļ�������Ŀ¼
	inline const QString m_repoTempDir = "\\env\\bat\\dist\\venv\\temp";      //��Ŵ����ֿ�ʱ������ʱ�ļ���Temp�ļ���Ŀ¼
	inline const QString m_currentPyExeDir = "\\env\\bat\\dist";              //���python��exe����Ŀ¼


	inline const QString m_registerTablePath = "HKEY_CURRENT_USER\\Software\\Cpp\\CppCheck";   //ע���·��
	inline const QString m_registerTableDirDefaultOpenPathKey = "defaultDirOpenPath";          //ע�����Ĭ�ϴ�·���ļ�

	inline const QString getOpenDirDefaultPath()
	{
		QSettings settings(m_registerTablePath, QSettings::NativeFormat);
		QString value = settings.value(m_registerTableDirDefaultOpenPathKey).toString();
		if(value.isEmpty())
			return QStandardPaths::DocumentsLocation;
		return value;
	}

	inline QString getFatherDirName()
	{
		if (m_fatherDirName.isEmpty())
		{
			//char fileBuffer[_MAX_PATH];
			//if (_fullpath(fileBuffer, __FILE__, _MAX_PATH) == nullptr)
			//	return "";

			//std::string filePathTemp = fileBuffer;

			//// ��ȡ��Ŀ¼·��  
			//std::size_t lastSlash = filePathTemp.find_last_of("/\\");
			//std::string m_fatherDirName1 = filePathTemp.substr(0, lastSlash);

			//m_fatherDirName = QString::fromStdString(m_fatherDirName1);

			QString filePath = QCoreApplication::applicationFilePath();
			QFileInfo fileInfo(filePath);
			m_fatherDirName = fileInfo.dir().path();
		}
		return m_fatherDirName;
	}
}
#endif
