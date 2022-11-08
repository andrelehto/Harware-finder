#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTextStream>
#include "wmi_api.h"

int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("Hardware-finder");
  QCoreApplication::setApplicationVersion("1.0");
  QCommandLineParser parser;

  parser.setApplicationDescription(
      "\nCLI tool to fetches hardware information from host-machine.");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption targetFileOption(
      QStringList() << "t"
                    << "target-file",
      QCoreApplication::translate(
          "main", "Target <file-path> to write hardware information to."),
      QCoreApplication::translate("main", "file-path"));
  parser.addOption(targetFileOption);

  parser.process(app);

  QString targetFile = parser.value(targetFileOption);

  QFileInfo fileInfo(targetFile);
  if (QFile::exists(targetFile)) {
    qDebug() << QString("[Hardware-finder] Error | File '" +
                        fileInfo.absoluteFilePath() + "' already exists")
                    .toLocal8Bit()
                    .constData();
    ;
    return 1;
  }

  QFile file(targetFile);
  qDebug() << QString("[Hardware-finder] Info | Creating file '" +
                      fileInfo.absoluteFilePath() + "'")
                  .toLocal8Bit()
                  .constData();

  WMIAPI* wmiapi = new WMIAPI();
  qDebug() << "[Hardware-finder] Info | Fetching hardware infomation...";
  wmiapi->runSearch();

  if (file.open(QIODevice::ReadWrite)) {
    QTextStream stream(&file);
    stream << "Generated by Hardware-finder 1.0 \n"
           << "\nGeneral info:"
           << "\n-------------------------------------------"
           << "\n Product ID: " << wmiapi->productID()
           << "\n Machine ID: " << wmiapi->machineID()
           << "\n OS: " << wmiapi->os() << "\n Build: " << wmiapi->build()
           << "\n Version: " << wmiapi->version() << "\n RAM: " << wmiapi->RAM()
           << "\n-------------------------------------------"
           << "\n\n"
           << "\nDisplay info:"
           << "\n-------------------------------------------"
           << "\nResolution: " << wmiapi->resolution()
           << "\nRefresh rate: " << wmiapi->refreshRate()
           << "\n-------------------------------------------"
           << "\n\n"
           << "\nProcessor info (CPU):"
           << "\n-------------------------------------------"
           << "\nCPU Name: " << wmiapi->CPUName()
           << "\nCPU Clock Speed: " << wmiapi->CPUClockSpeed()
           << "\n-------------------------------------------"
           << "\n\n"
           << "\nGraphic card info (GPU):"
           << "\n-------------------------------------------";

    for (const auto& GPU : wmiapi->GPUs()) {
      stream << "\nName: " << GPU.name
             << "\nDriver version: " << GPU.driverVersion
             << "\nVRAM: " << GPU.VRAM
             << "\n-------------------------------------------";
    }
  }
  qDebug() << QString(
                  "[Hardware-finder] Info | Hardware information has been "
                  "written to '" +
                  fileInfo.absoluteFilePath() + "'")
                  .toLocal8Bit()
                  .constData();
  return 0;
}
