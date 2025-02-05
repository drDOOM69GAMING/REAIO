#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QStandardPaths>
#include <QDateTime>
#include <QTextCharFormat>
#include <QtConcurrent>
#include <QPluginLoader>
#include <QDebug>
#include <QImageReader>
#include "quazip.h"
#include "quazipfile.h"

class REInstaller : public QWidget {
    Q_OBJECT

public:
    REInstaller(QWidget *parent = nullptr)
        : QWidget(parent), manager(new QNetworkAccessManager(this))
    {
        initUI();
        setupConnections();
        initGameData();

        // Set a default game so an image is loaded immediately.
        handleGameSelection("RE1");

        // Use a single-shot timer to update the background image after the widget is shown.
        QTimer::singleShot(0, this, [this]() {
            updateBackgroundImage();
        });
    }

private slots:
    void handleGameSelection(const QString &game) {
        currentGame = game;
        updateUIForGame();
    }

    void handleModDownload() {
        QUrl url = modDropdown->currentData().toUrl();
        if (url.isValid()) {
            logMessage("Attempting to open URL: " + url.toString());
            if (!QDesktopServices::openUrl(url)) {
                logMessage("Failed to open URL: " + url.toString(), true);
            }
        } else {
            logMessage("Invalid download URL selected.", true);
        }
    }

    void handleAutoProcess() { startProcess("auto"); }
    void handleAmdProcess() { startProcess("amd"); }
    void handleNvidiaProcess() { startProcess("nvidia"); }

    void handleBlink() {
        blinkState = !blinkState;
        currentButton->setStyleSheet(blinkState
            ? "background-color: green; color: white;"
            : "background-color: #333333; color: white;");
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        QWidget::resizeEvent(event);
        updateBackgroundImage();
    }

private:
    struct GameConfig {
        QString folderName;
        QString zipName;
        QString dataPath;
        QString finalName;
        QString appDataFolder;
        QString savedataPath;
        QVector<QString> additionalZips;
    };

    QNetworkAccessManager *manager;
    QTextEdit *logArea;
    QComboBox *modDropdown;
    QLabel *backgroundLabel;
    QTimer blinkTimer;
    QPushButton *currentButton;
    QString currentGame;
    QString currentBgPath;
    QMap<QString, GameConfig> gameData;
    bool blinkState = false;
    QPushButton *re1Button, *re2Button, *re3Button;
    QPushButton *autoButton, *amdButton, *nvidiaButton;

    void initUI() {
        setWindowTitle("Resident Evil AIO Installer");
        setStyleSheet("background-color: #2E2E2E; color: white;");
        resize(1000, 800);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Menu bar
        QMenuBar *menuBar = new QMenuBar(this);
        QMenu *helpMenu = menuBar->addMenu("Help");
        helpMenu->addAction("Show Help", this, &REInstaller::showHelp);
        mainLayout->setMenuBar(menuBar);

        // Game selection buttons
        QHBoxLayout *gameLayout = new QHBoxLayout();
        re1Button = new QPushButton("Resident Evil 1", this);
        re2Button = new QPushButton("Resident Evil 2", this);
        re3Button = new QPushButton("Resident Evil 3", this);
        for (auto btn : {re1Button, re2Button, re3Button}) {
            btn->setStyleSheet("background-color: #333333; color: white;");
            gameLayout->addWidget(btn);
        }
        mainLayout->addLayout(gameLayout);

        // Background image
        backgroundLabel = new QLabel(this);
        backgroundLabel->setAlignment(Qt::AlignCenter);
        backgroundLabel->setMinimumHeight(400);
        mainLayout->addWidget(backgroundLabel);

        // Mod selection
        QHBoxLayout *modLayout = new QHBoxLayout();
        modDropdown = new QComboBox(this);
        modDropdown->addItem("Biohazard_mod.zip", QUrl("https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Biohazard_mod.zip"));
        modDropdown->addItem("Bio2_mod.zip", QUrl("https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Bio2_mod.zip"));
        modDropdown->addItem("Bio3_mod.zip", QUrl("https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Bio3_mod.zip"));
        modDropdown->addItem("dgVoodoo_AMD_fix.zip", QUrl("https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/dgVoodoo_AMD_fix.zip"));
        QPushButton *downloadBtn = new QPushButton("Download Selected Mod", this);
        connect(downloadBtn, &QPushButton::clicked, this, &REInstaller::handleModDownload);
        modLayout->addWidget(modDropdown);
        modLayout->addWidget(downloadBtn);
        mainLayout->addLayout(modLayout);

        // Log area
        logArea = new QTextEdit(this);
        logArea->setReadOnly(true);
        logArea->setStyleSheet("background-color: #1E1E1E; color: white;");
        mainLayout->addWidget(logArea);

        // Process buttons
        QHBoxLayout *btnLayout = new QHBoxLayout();
        autoButton = new QPushButton("Auto", this);
        amdButton = new QPushButton("Auto AMD", this);
        nvidiaButton = new QPushButton("Auto Nvidia", this);
        for (auto btn : {autoButton, amdButton, nvidiaButton}) {
            btn->setStyleSheet("background-color: #333333; color: white;");
            btn->setVisible(false);
            btnLayout->addWidget(btn);
        }
        mainLayout->addLayout(btnLayout);

        // Credits
        QLabel *credits = new QLabel("Credits: TeamX, RESHDP, Gemini");
        credits->setStyleSheet("font-family: Arial; font-size: 12px;");
        mainLayout->addWidget(credits);

        connect(&blinkTimer, &QTimer::timeout, this, &REInstaller::handleBlink);
    }

    void setupConnections() {
        connect(re1Button, &QPushButton::clicked, [this]() { handleGameSelection("RE1"); });
        connect(re2Button, &QPushButton::clicked, [this]() { handleGameSelection("RE2"); });
        connect(re3Button, &QPushButton::clicked, [this]() { handleGameSelection("RE3"); });
        connect(autoButton, &QPushButton::clicked, this, &REInstaller::handleAutoProcess);
        connect(amdButton, &QPushButton::clicked, this, &REInstaller::handleAmdProcess);
        connect(nvidiaButton, &QPushButton::clicked, this, &REInstaller::handleNvidiaProcess);
    }

    void initGameData() {
        gameData = {
            {"RE1", {"BIOHAZARD Mediakite", "Biohazard_mod.zip", "/horr", "RE1SHDP - RE Seamless HD Project", "BIO1QtBackground", "/horr/savedata", {"dgVoodoo_AMD_fix.zip"}}},
            {"RE2", {"biohazard-2-apan-source-next", "Bio2_mod.zip", "/data", "RE2SHDP - RE Seamless HD Project", "BIO2QtBackground", "/data/savedata", {}}},
            {"RE3", {"Bio Hazard 3 (SOURCENEXT)", "Bio3_mod.zip", "/data", "RE3SHDP - RE Seamless HD Project", "BIO3QtBackground", "/data/savedata", {}}}
        };
    }

    void updateUIForGame() {
        const GameConfig &config = gameData[currentGame];
        autoButton->setVisible(currentGame != "RE1");
        amdButton->setVisible(currentGame == "RE1");
        nvidiaButton->setVisible(currentGame == "RE1");

        // Build the background image path using native separators.
        // Note: Changed from ".jpg" to ".png"
        QString localBgPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/" + currentGame + ".png");
        QFileInfo checkFile(localBgPath);
        if (checkFile.exists() && checkFile.isFile()) {
            currentBgPath = localBgPath;
            logMessage("Loading background image from: " + currentBgPath);
            updateBackgroundImage();
        } else {
            logMessage("Background image not found at: " + localBgPath, true);
            backgroundLabel->setText("Image not available");
        }
    }

    void startProcess(const QString &processType) {
        currentButton = qobject_cast<QPushButton *>(sender());
        blinkTimer.start(500);
        currentButton->setEnabled(false);

        QtConcurrent::run([this, processType]() {
            try {
                const GameConfig &config = gameData[currentGame];

                // Locate game directory
                QString gameDir = findGameDirectory(config.folderName);
                if (gameDir.isEmpty()) throw std::runtime_error("Game directory not found");

                // Handle additional zips for AMD
                if (processType == "amd" && !config.additionalZips.isEmpty()) {
                    for (const QString &zip : config.additionalZips) {
                        QString zipPath = locateModFile(zip);
                        QString tempAmd = gameDir + "/temp_amd";
                        extractFiles(zipPath, tempAmd);
                        
                        // Check if the temp_amd folder contains a single subdirectory.
                        QDir tempAmdDir(tempAmd);
                        QStringList subDirsAmd = tempAmdDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
                        if (subDirsAmd.size() == 1) {
                            QString singleDir = tempAmdDir.absoluteFilePath(subDirsAmd.first());
                            logMessage("Using subdirectory " + singleDir + " for AMD additional files.");
                            tempAmd = singleDir;
                        }
                        
                        copyFilesFromTempToGame(tempAmd, gameDir + config.dataPath);
                        QDir(gameDir + "/temp_amd").removeRecursively();
                    }
                }

                // Main mod installation
                QString modPath = locateModFile(config.zipName);
                QString tempDir = gameDir + "/temp";
                extractFiles(modPath, tempDir);

                // Check if the temp folder contains a single subdirectory.
                // If so, use that subdirectory as the source instead.
                QDir tempQDir(tempDir);
                QStringList subDirs = tempQDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
                if (subDirs.size() == 1) {
                    QString singleDir = tempQDir.absoluteFilePath(subDirs.first());
                    logMessage("Using subdirectory " + singleDir + " as source for copying files.");
                    tempDir = singleDir;
                }

                // Copy files from temp (or the adjusted folder) to the correct folder (horr or data)
                QString targetFolder = gameDir + "/" + config.dataPath;
                copyFilesFromTempToGame(tempDir, targetFolder);

                // Clean up temp directory
                QDir(gameDir + "/temp").removeRecursively();

                // For RE1 with AMD or NVIDIA, create an empty "Savedata" folder inside the "horr" folder.
                if (currentGame == "RE1" && (processType == "amd" || processType == "nvidia")) {
                    QString savedataFolder = targetFolder + "/Savedata";
                    if (QDir().mkpath(savedataFolder)) {
                        logMessage("Created empty folder: " + savedataFolder);
                    } else {
                        logMessage("Failed to create folder: " + savedataFolder, true);
                    }
                }

                // Final cleanup and renaming
                finalizeInstallation(gameDir, config.finalName, processType == "amd");

                logMessage("Process completed successfully");
            } catch (const std::exception &e) {
                logMessage(QString("Error: ") + e.what(), true);
            }

            QMetaObject::invokeMethod(this, [this]() {
                blinkTimer.stop();
                currentButton->setEnabled(true);
                currentButton->setStyleSheet("background-color: #333333; color: white;");
            });
        });
    }

    QString findGameDirectory(const QString &folderName) {
        QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QDir desktopDir(desktop);
        if (desktopDir.exists(folderName)) {
            return desktopDir.filePath(folderName);
        }
        throw std::runtime_error("Game directory not found: " + folderName.toStdString());
    }

    QString locateModFile(const QString &fileName) {
        QStringList locations = {
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
            QCoreApplication::applicationDirPath()
        };
        for (const QString &loc : locations) {
            QFileInfo file(loc + "/" + fileName);
            if (file.exists()) return file.absoluteFilePath();
        }
        throw std::runtime_error("Mod file not found: " + fileName.toStdString());
    }

    void extractFiles(const QString &zipPath, const QString &destDir) {
        QuaZip zip(zipPath);
        if (!zip.open(QuaZip::mdUnzip)) {
            logMessage("Failed to open zip file: " + zipPath, true);
            return;
        }
        QuaZipFile file(&zip);
        for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
            QString fileName = zip.getCurrentFileName();
            if (fileName.endsWith('/')) continue; // Skip directories

            QString filePath = QDir(destDir).absoluteFilePath(fileName);
            QFileInfo fi(filePath);
            QDir().mkpath(fi.absolutePath()); // Ensure the directory exists

            if (!file.open(QIODevice::ReadOnly)) {
                logMessage("Skipping file (open failed): " + fileName, true);
                continue;
            }

            QFile output(filePath);
            if (output.open(QIODevice::WriteOnly)) {
                output.write(file.readAll());
                logMessage("Extracted: " + fileName);
            } else {
                logMessage("Failed to write: " + fileName, true);
            }
            file.close();
        }
        zip.close();
    }

    void copyFilesFromTempToGame(const QString &src, const QString &dest) {
        QDir source(src);
        QDir destination(dest);

        if (!destination.exists()) {
            QDir().mkpath(dest); // Ensure the destination directory exists
        }

        // Copy all files and directories recursively
        for (const QString &entry : source.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
            QString srcPath = source.absoluteFilePath(entry);
            QString destPath = destination.absoluteFilePath(entry);

            if (QFileInfo(srcPath).isDir()) {
                // Recursively copy subdirectories
                copyFilesFromTempToGame(srcPath, destPath);
            } else {
                // Copy individual files
                if (QFile::exists(destPath)) {
                    QFile::remove(destPath); // Overwrite existing files
                }
                if (QFile::copy(srcPath, destPath)) {
                    logMessage("Copied: " + entry);
                } else {
                    logMessage("Failed to copy: " + entry, true);
                }
            }
        }
    }

    void finalizeInstallation(const QString &gameDir, const QString &finalName, bool isAmd = false) {
        QString desktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString targetFolder = gameDir + "/" + (currentGame == "RE1" ? "horr" : "data");
        QString finalPath = desktop + "/" + finalName;

        // Step 1: Rename the target folder
        if (QDir(finalPath).exists()) {
            QDir(finalPath).removeRecursively();
        }
        if (!QDir().rename(targetFolder, finalPath)) {
            throw std::runtime_error("Failed to move and rename folder");
        }

        // Step 2: Delete the original parent folder
        if (QDir(gameDir).exists()) {
            QDir(gameDir).removeRecursively();
        }

        // Step 3: Clean up leftover .zip files (only delete the mod ZIP corresponding to the selected game)
        QStringList modFiles = {gameData[currentGame].zipName};
        if (isAmd) {
            modFiles.append("dgVoodoo_AMD_fix.zip");
        }
        for (const QString &file : modFiles) {
            QFile::remove(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + file);
            QFile::remove(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + file);
        }

        logMessage("Cleanup completed successfully");
    }

    void updateBackgroundImage() {
        // Print out the supported image formats for debugging
        QList<QByteArray> formats = QImageReader::supportedImageFormats();
        qDebug() << "Supported image formats:" << formats;

        // Also print out the current library paths
        qDebug() << "Qt library paths:" << QCoreApplication::libraryPaths();

        if (QFile::exists(currentBgPath)) {
            QPixmap pixmap;
            // Explicitly try loading as "PNG"
            bool loaded = pixmap.load(currentBgPath, "PNG");
            if (!loaded) {
                logMessage("Failed to load background image: " + currentBgPath, true);
                backgroundLabel->setText("Invalid image format");
            } else {
                pixmap = pixmap.scaled(backgroundLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                backgroundLabel->setPixmap(pixmap);
                backgroundLabel->show();
            }
        } else {
            logMessage("Background image file not found: " + currentBgPath, true);
            backgroundLabel->setText("Image not available");
        }
    }

    void logMessage(const QString &message, bool error = false) {
        QMetaObject::invokeMethod(this, [this, message, error]() {
            QTextCharFormat format;
            format.setForeground(error ? Qt::red : Qt::white);
            logArea->setCurrentCharFormat(format);
            logArea->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ") + message);
        });
    }

    void showHelp() {
        QMessageBox::information(this, "Help",
            "Game Requirements:\n\n"
            "- Japanese versions required\n"
            "- Folders must be on Desktop\n"
            "- Correct folder names:\n"
            "  • RE1: BIOHAZARD Mediakite\n"
            "  • RE2: biohazard-2-apan-source-next\n"
            "  • RE3: Bio Hazard 3 (SOURCENEXT)\n\n"
            "Mod ZIPs should be in Downloads or Desktop");
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("REModTeam");
    app.setApplicationName("RE_AIO_Installer");

    // Make sure the plugins folder is added to the library path.
    QString pluginPath = QCoreApplication::applicationDirPath() + "/plugins";
    QCoreApplication::addLibraryPath(pluginPath);
    qDebug() << "Added plugins path:" << pluginPath;
    qDebug() << "Current library paths:" << QCoreApplication::libraryPaths();

    // Optionally set the working directory to the application directory.
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    REInstaller installer;
    installer.show();
    return app.exec();
}

#include "REAIO.moc"
