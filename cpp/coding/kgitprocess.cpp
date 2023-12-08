#include "kgitprocess.h"
#include "kglobaldata.h"

#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QTextCodec>

KGitProcess::KGitProcess()
{
}

KGitProcess::~KGitProcess()
{
}

void KGitProcess::run()
{
	QString exePath = KGlobalDataNameSpace::getFatherDirName() + m_path;

	QStringList arguments;	
	for (const QString& arg : m_vec)
	{ 
		arguments.append(arg);
	}

	QDir::setCurrent(exePath);

	QProcess process; 
	process.setProcessChannelMode(QProcess::MergedChannels);
	process.start(exePath, arguments);
	process.waitForFinished(-1);      //等待终端输出无限秒

	QByteArray outputData = process.readAll();
	QTextCodec* codec = QTextCodec::codecForLocale();         //获取当前系统本地编码对象
	if (codec == nullptr)
		return;
	m_output = codec->toUnicode(outputData);            //将字节流解码为Unicode字符
}

void KGitProcess::checkOrgNameExist(const QString& orgName)
{
	m_vec.clear();
	m_vec.push_back("checkOrgExist");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--org_name");
	m_vec.push_back(orgName);

	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::createOrg(const QString& orgName, const QString& describe)
{
	m_vec.clear();
	m_vec.push_back("createOrg");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--org_name");
	m_vec.push_back(orgName);
	m_vec.push_back("--org_description");
	m_vec.push_back(describe);

	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::createUser()
{
	m_vec.clear();
	m_vec.push_back("createUser");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--current_path");
	m_vec.push_back(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_currentPyExeDir);

	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::createRepo(const QString& orgName, const QString& privateRepo,
	const QString& isAddDefaultFile, const QString& defaultFilePath)
{
	m_vec.clear();
	m_vec.push_back("createRepo");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--org_name");
	m_vec.push_back(orgName);
	m_vec.push_back("--current_path");
    m_vec.push_back(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_currentPyExeDir);
	m_vec.push_back("--repo_private");
	m_vec.push_back(privateRepo);
	m_vec.push_back("--isSetdefaultfile");
	m_vec.push_back(isAddDefaultFile);
	m_vec.push_back("--repo_defaultFile");
	m_vec.push_back(defaultFilePath);


	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::bindUserAndRepo(const QString& orgName)
{
	m_vec.clear();
	m_vec.push_back("bindUserAndRepo");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--org_name");
	m_vec.push_back(orgName);
	m_vec.push_back("--current_path");
	m_vec.push_back(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_currentPyExeDir);
	
	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::pullCode(const QString& orgName, const QString& outputDir)
{
	m_vec.clear();
	m_vec.push_back("pullCode");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--org_name");
	m_vec.push_back(orgName);
	m_vec.push_back("--pullCodeOutPath");
	m_vec.push_back(outputDir);
	

	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::delRepoAndUser(const QString& orgName, const QString& stringToPyTool)
{
	m_vec.clear();
	m_vec.push_back("delete");
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);
	m_vec.push_back("--org_name");
	m_vec.push_back(orgName);
	m_vec.push_back("--current_path");
	m_vec.push_back(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_currentPyExeDir);
	m_vec.push_back("--deleteMenu");
	m_vec.push_back(stringToPyTool);

	m_path = KGlobalDataNameSpace::m_wpsgogsgitExeDir;
}

void KGitProcess::inputHighLightFileArg(const QString& filepath,const QString& background, const QString& fontSize)
{
	m_vec.clear();
	m_vec.push_back(filepath);
	m_vec.push_back(background);
	m_vec.push_back(fontSize);

	m_vec.push_back(KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_currentPyExeDir);

	m_path = KGlobalDataNameSpace::m_highlightExeDir;
}

void KGitProcess::inputJplagArg(const QString path, const QString& language)
{
	m_vec.clear();
	m_vec.push_back("-l");
	m_vec.push_back(language);
	m_vec.push_back(path);

	m_path = KGlobalDataNameSpace::m_jplagExeDir;
}

QString KGitProcess::getOutput()
{
	return m_output;
}


// --TODO set/get方法可以用property解决
