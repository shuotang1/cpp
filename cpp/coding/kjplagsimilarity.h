// ------------------------------------------------------------
// 2023/10/10
// kjplagsimilarity.h
// 功能描述：检测给定目录下C++文件的文件相似度
// -------------------------------------------------------------

#ifndef __KJPLAGSIMILARITY_KJPLAGSIMILARITY_H__
#define __KJPLAGSIMILARITY_KJPLAGSIMILARITY_H__

#include <QString>
#include <QMap>

struct SimilarityResults
{
	QString firstFileName;
	QString secondFileName;
	QString similarity;
	double similarityValue;
};

class KJplagSimilarity
{
public:
	KJplagSimilarity(const QString& path);
	~KJplagSimilarity();

	void outputDirectorySimilarity(const QString& dirName, QVector<SimilarityResults>& SimilarityVecResults);
	void outputSpecificDirSim(const QString& dirName, const QString& excludeName,
		const QString& lanuage, QVector<SimilarityResults>& SimilarityVecResults);
	void outputTwoFileCompareSim(const QString& firFilePath, const QString& secFilePath, QVector<SimilarityResults>& SimilarityVecResults);
	void outputTwoDirCompareSim(const QString& firDirPath, const QString& secDirPath, QVector<SimilarityResults>& SimilarityVecResults);

	void processDirectory(const std::string& directoryPath);      //递归处理目录，如果该文件是UTF-8 BOM编码，将其转换为UTF-8编码
private:
	KJplagSimilarity(const KJplagSimilarity& other) = default;
	void operator=(const KJplagSimilarity& other) = delete;
	KJplagSimilarity(const KJplagSimilarity&& other) = delete;
	KJplagSimilarity& operator=(KJplagSimilarity&& other) = default;

	void processCommand(const QString path, QVector<SimilarityResults>& SimilarityVecResults, const QString& language = "cpp");
	void sortSimResult(QVector<SimilarityResults>& SimilarityVecResults);
	QString getJplagPath();
	void addComparisonContents(QString& fileName, double similarity, SimilarityResults& comparison);//对终端内容进行处理，成为QTableView需要样式

	void removeBomCharacter(std::string& filePath);               //如果该文件是UTF-8 BOM编码，将其转换为UTF-8编码

	void moveDirectoryFromSourceToDes(const QString& source, const QString& des, const QString& exclude);  //将源目录下所有文件剪切到目标目录下
	void moveDirectoryFromDesToSource(const QString& source, const QString& des);

	QString m_path;

	QMap<QString, QString> m_sourceToDesMap;     //key:源目录在目标目录名称   value：源目录名称
};

#endif 


