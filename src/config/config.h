#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    explicit Config(const QString &path, const QString &name);

    QString filePath() const;
    QString name() const;

    void setFilePath(const QString &path);
    void setName(const QString &name);

signals:

private:
    QString m_filePath;
    QString m_name;
};

#endif // CONFIG_H
