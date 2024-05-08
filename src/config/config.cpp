#include "config.h"

Config::Config(const QString &path, const QString &name)
    : m_filePath{path}, m_name{name}
{}

QString Config::filePath() const
{
    return m_filePath;
}

QString Config::name() const
{
    return m_name;
}

void Config::setFilePath(const QString &path)
{
    m_filePath = path;
}

void Config::setName(const QString &name)
{
    m_name = name;
}

