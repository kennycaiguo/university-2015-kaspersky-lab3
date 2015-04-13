#ifndef SCANNERMAINWINDOW_H
#define SCANNERMAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class ScannerMainWindow : public QMainWindow
{
    Q_OBJECT

    QMenuBar *mnuBar;
    QMenu *menuHistory;
    QMenu *menuRecent;
    QMenu *mnuHelp;
    QAction *actionAbout;
    QAction *actionClearHistory;
    QAction *actionClearLog;

    QWidget *buttonWidget;
    QPushButton *btnChooseFile;
    QPushButton *btnChooseDir;
    QPushButton *btnStart;

    QGroupBox *actionBox;
    QRadioButton *radioAsk;
    QRadioButton *radioIgnore;
    QRadioButton *radioDelete;

    QGroupBox *optionsBox;
    QLineEdit *extensionEdit;
    QCheckBox *recursiveBox;

    QWidget *topControls;

    QLabel *lblFileName;
    QLabel *labelLog;
    QLineEdit *editPath;
    QTextEdit *editLog;

    QWidget *centralWidget;

    void initMenu();
    void initButtons();
    void initOptions();
    void initAction();
    void initTopControls();
    void initBottomWidgets();
    void initCentralWidget();

public slots:
    void mnuAboutTriggered();
    void onSelectFile();
    void onSelectFolder();

public:
    ScannerMainWindow(QWidget *parent = 0);
    ~ScannerMainWindow();
    void initLayout();
    void Connect();
};

#endif // SCANNERMAINWINDOW_H
