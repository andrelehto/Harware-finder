#include <QCommandLineParser>
#include <QCoreApplication>
int main(int argc, char* argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("Hardware-finder");
  QCoreApplication::setApplicationVersion("1.0");
  QCommandLineParser parser;

  parser.setApplicationDescription("CLI tool to read out hardware info.");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption targetFileOption(
      QStringList() << "t"
                    << "target-file",
      QCoreApplication::translate(
          "main", "Target <file-path> to write hardware-info to."),
      QCoreApplication::translate("main", "file-path"));
  parser.addOption(targetFileOption);

  parser.process(app);
  return 0;
}
