#include "scannermainwindow.h"

ScannerMainWindow::ScannerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QCoreApplication::setOrganizationName("MSU");
    QCoreApplication::setApplicationName("SimpleAntivirusScanner");

    pSettings = new QSettings();
}

ScannerMainWindow::~ScannerMainWindow()
{

}

void ScannerMainWindow::initMenu()
{
    mnuBar = new QMenuBar(wgtCentral);
    mnuHistory = new QMenu(wgtCentral);
    mnuRecent = new QMenu(wgtCentral);
    mnuHelp = new QMenu(wgtCentral);
    actClearHistory = new QAction(wgtCentral);
    actClearLog = new QAction(wgtCentral);
    actAbout = new QAction(wgtCentral);

    mnuBar->addMenu(mnuHistory);
    mnuBar->addMenu(mnuHelp);
    mnuHistory->addMenu(mnuRecent);
    mnuHistory->addAction(actClearHistory);
    mnuHistory->addAction(actClearLog);
    mnuHelp->addAction(actAbout);

    mnuHistory->setTitle("History");
    mnuRecent->setTitle("Recent");
    mnuHelp->setTitle("Help");
    actClearHistory->setText("Clear scan history");
    actClearLog->setText("Clear scan log");
    actAbout->setText("About...");
}

void ScannerMainWindow::initButtons()
{
    wgtButtons = new QWidget(wgtTop);
    btnSelectFile = new QPushButton(wgtButtons);
    btnSelectFolder = new QPushButton(wgtButtons);
    btnCheck = new QPushButton(wgtButtons);
    QVBoxLayout *vbox = new QVBoxLayout(wgtButtons);

    btnSelectFile->setText("Select File");
    btnSelectFolder->setText("Select Directory");
    btnCheck->setText("Start");

    wgtButtons->setLayout(vbox);
    vbox->addWidget(btnSelectFile);
    vbox->addWidget(btnSelectFolder);
    vbox->addWidget(btnCheck);
}

void ScannerMainWindow::initOptions()
{
    gbxOptions = new QGroupBox(wgtTop);
    QLabel *lblExtensions = new QLabel(gbxOptions);
    edtExtensions = new QLineEdit(gbxOptions);
    chkRecursive = new QCheckBox(gbxOptions);
    QVBoxLayout *vbox = new QVBoxLayout(gbxOptions);

    gbxOptions->setTitle("Scan Options");
    lblExtensions->setText("Filter by extension");
    edtExtensions->setText("*.*");
    chkRecursive->setText("Scan subdirectories recursively");

    gbxOptions->setLayout(vbox);
    vbox->addWidget(lblExtensions);
    vbox->addWidget(edtExtensions);
    vbox->addWidget(chkRecursive);
}

void ScannerMainWindow::initAction()
{
    gbxActions = new QGroupBox(wgtTop);
    QVBoxLayout *vbox = new QVBoxLayout(gbxActions);
    rdoAsk = new QRadioButton(gbxActions);
    rdoIgnore = new QRadioButton(gbxActions);
    rdoDelete = new QRadioButton(gbxActions);

    gbxActions->setTitle("Action");
    rdoAsk->setText("Ask");
    rdoIgnore->setText("Ignore");
    rdoDelete->setText("Delete");

    gbxActions->setLayout(vbox);
    vbox->addWidget(rdoAsk);
    vbox->addWidget(rdoIgnore);
    vbox->addWidget(rdoDelete);

    rdoAsk->setChecked(true);
}

void ScannerMainWindow::initTopWidget()
{
    wgtTop = new QWidget(wgtCentral);

    initButtons();
    initOptions();
    initAction();

    QHBoxLayout *hbox = new QHBoxLayout(wgtTop);
    wgtTop->setLayout(hbox);
    hbox->addWidget(wgtButtons);
    hbox->addWidget(gbxOptions);
    hbox->addWidget(gbxActions);
    hbox->addStretch();
    wgtTop->setFixedWidth(500);
}

void ScannerMainWindow::initBottomWidgets()
{
    lblFileName = new QLabel(wgtCentral);
    lblLog = new QLabel(wgtCentral);
    edtFileName = new QLineEdit(wgtCentral);
    edtLog = new QTextEdit(wgtCentral);
    edtLog->setReadOnly(true);
    lblFileName->setText("File / Directory");
    lblLog->setText("Scan log");
}

void ScannerMainWindow::initCentralWidget()
{
    wgtCentral = new QWidget(this);
    setCentralWidget(wgtCentral);

    initMenu();
    initTopWidget();
    initBottomWidgets();

    QVBoxLayout *vbox = new QVBoxLayout(wgtCentral);
    wgtCentral->setLayout(vbox);
    vbox->addWidget(mnuBar);
    vbox->addWidget(wgtTop);
    vbox->addWidget(lblFileName);
    vbox->addWidget(edtFileName);
    vbox->addWidget(lblLog);
    vbox->addWidget(edtLog);
}

void ScannerMainWindow::initLayout()
{
    setWindowTitle("Simple Antivirus Scanner");
    initCentralWidget();
    loadSettings();
}

void ScannerMainWindow::Connect()
{
    connect(actAbout, SIGNAL(triggered()), this, SLOT(mnuAboutTriggered()));
    connect(actClearLog, SIGNAL(triggered()), edtLog, SLOT(clear()));
    connect(actClearHistory, SIGNAL(triggered()), this, SLOT(onClearHistory()));
    connect(btnSelectFile, SIGNAL(clicked()), this, SLOT(onSelectFile()));
    connect(btnSelectFolder, SIGNAL(clicked()), this, SLOT(onSelectFolder()));
    connect(btnCheck, SIGNAL(clicked()), this, SLOT(onCheck()));
    connect(&scanner, SIGNAL(log(QString)), this, SLOT(onLog(QString)));
}

void ScannerMainWindow::mnuAboutTriggered()
{
    QMessageBox msg;
    QString s;
    s = "Проект SimpleAntivirusScanner_KL2015 выполнен в рамках спецкурса"
        " Лаборатории Касперского \"Командная разработка кроссплатформенных"
        " приложений\"."
        "\n"
        "\n"
        "Авторы: Богданенко Алексей, Хамитов Камиль"
        "\n"
        "\n"
        "Апрель 2015";

    msg.setText(s);
    msg.exec();
}

void ScannerMainWindow::onSelectFile()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QDir::homePath());

    if (dialog.exec()) {
        QStringList filenames = dialog.selectedFiles();
        QString filename = filenames[0];
        edtFileName->setText(filename);
    }
}

void ScannerMainWindow::onSelectFolder()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory(QDir::homePath());
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    if (dialog.exec()) {
        QStringList filenames = dialog.selectedFiles();
        QString filename = filenames[0];
        edtFileName->setText(filename);
    }
}

void ScannerMainWindow::onLog(QString aHtmlMessage)
{
    qApp->processEvents();
    edtLog->moveCursor(QTextCursor::End);
    edtLog->insertHtml(aHtmlMessage);
    QString newline = "<br>\n";
    edtLog->insertHtml(newline);
}

void ScannerMainWindow::onCheck()
{
    QString aTargetName = edtFileName->text();
    bool aIsRecursive = chkRecursive->isChecked();
    QString aExt = edtExtensions->text();

    MalwarePolicy policy = MPOL_SKIP;
    if (rdoDelete->isChecked()) {
        policy = MPOL_REMOVE;
    }

    if (rdoAsk->isChecked()) {
        policy = MPOL_ASK;
    }

    scanner.setPolicy(policy);
    scanner.check(aTargetName, aIsRecursive, aExt);

    addRecent(aTargetName);
}

void ScannerMainWindow::addRecent(QString filename)
{
    QAction *actLastFileName = new QAction(mnuRecent);
    mnuRecent->addAction(actLastFileName);
    actLastFileName->setText(filename);
    recentFileNames.append(filename);
    connect(actLastFileName, SIGNAL(triggered()), this, SLOT(onRecent()));
}

void ScannerMainWindow::onRecent()
{
    QObject *sender = QObject::sender();
    QAction *action = qobject_cast<QAction*>(sender);
    QString fileName = action->text();
    edtFileName->setText(fileName);
}

void ScannerMainWindow::onClearHistory()
{
    mnuRecent->clear();
    recentFileNames.clear();
}

void ScannerMainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void ScannerMainWindow::saveSettings()
{
    pSettings->setValue("options/extensions", edtExtensions->text());
    pSettings->setValue("options/recursive", chkRecursive->isChecked());

    QString action = "skip";
    if (rdoDelete->isChecked()) {
        action = "remove";
    }

    if (rdoAsk->isChecked()) {
        action = "ask";
    }

    pSettings->setValue("action", action);
    pSettings->setValue("history", recentFileNames);
}

void ScannerMainWindow::loadSettings()
{
    edtExtensions->setText(pSettings->value("options/extensions", "*.*").toString());
    chkRecursive->setChecked(pSettings->value("options/recursive", false).toBool());

    QString action = pSettings->value("action").toString();
    rdoIgnore->setChecked(action == "skip");
    rdoDelete->setChecked(action == "remove");
    rdoAsk->setChecked(action == "ask");

    QStringList filenames = pSettings->value("history", QStringList()).toStringList();
    foreach(QString filename, filenames) {
        addRecent(filename);
    }
}
