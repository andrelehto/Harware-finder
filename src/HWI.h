#include <QProcess>

class HWI {
 public:
  struct GPU {
    QString name, driverVersion, VRAM;
  };

  static QString productID();
  static QString machineID();
  static QString os();
  static QString version();
  static QString build();
  static QString RAM();
  static QString CPUName();
  static QString CPUClockSpeed();
  static QString resolution();
  static QString refreshRate();
  static QVector<GPU> GPUs();

 private:
  static QString runProcess(QString target);
  static QString removeHeader(QString string);
  static QString convertToGB(QString bytes);
};