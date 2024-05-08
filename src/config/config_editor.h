#ifndef CONFIG_EDITOR_H
#define CONFIG_EDITOR_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConfigEditor;
}
QT_END_NAMESPACE

class ConfigEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigEditor(QWidget *parent = nullptr);
    ~ConfigEditor();

    void addFile();
    void openFile();
    void openFile(int index, const QString &filePath, const QString &title);

signals:
    void configFileSaved(const QString &filePath, const QString &title);
    void editedConfigFileSaved(int index, const QString &filePath, const QString &title);

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    enum class EditMode {ImportConfig, AddNewConfig, EditConfig};

    void saveConfigFile(const QString &configContent);
    // Use timestamp as saved file name
    QString generateFileName();

    Ui::ConfigEditor *ui;
    EditMode m_editMode;
    int m_configFileIndex;
    QString m_configFilePath;
};

#endif // CONFIG_EDITOR_H
