
class FileSelect : public QFileDialog
{
    Q_OBJECT;
public:
    FileSelect (QWidget *parent, const char *name, const char *filter);
    ~FileSelect();
private:
    QCheckBox *readonly;
};
