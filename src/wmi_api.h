#include <QProcess>

#define Win32_ComputerSystemProduct(target)                                   \
  "Get-CimInstance -ClassName Win32_ComputerSystemProduct | Select-Object " + \
      target
#define Win32_ComputerSystem(target) \
  "Get-CimInstance -ClassName Win32_ComputerSystem | Select-Object " + target
#define Win32_OperatingSystem(target) \
  "Get-CimInstance -ClassName Win32_OperatingSystem | Select-Object " + target
#define Win32_Processor(target) \
  "Get-CimInstance -ClassName Win32_Processor | Select-Object " + target
#define Win32_VideoController(target) \
  "Get-CimInstance -ClassName Win32_VideoController | Select-Object " + target

class WMIAPI : public QObject {
  Q_OBJECT

 public:
  explicit WMIAPI(QObject* parent = nullptr);

  struct GPU {
    QString name, driverVersion, VRAM;
  };

  void runSearch();

  QString productID();
  QString machineID();
  QString os();
  QString version();
  QString build();
  QString RAM();
  QString CPUName();
  QString CPUClockSpeed();
  QString resolution();
  QString refreshRate();
  QVector<GPU> GPUs();

 private:
  enum class Info {
    ProductID,
    MachineID,
    OS,
    Version,
    Build,
    Ram,
    CpuName,
    CpuClockSpeed,
    ResolutionVertical,
    ResolutionHorizontal,
    RefreshRateMin,
    RefreshRateMax,
    GpuNames,
    GpuVersions,
    GpuVram
  };

  struct Task {
    QProcess* process;
    QString target;
    Info type;
  };

  QString _productID, _machineID, _os, _version, _build, _ram, _cpuName,
      _cpuClockSpeed, _resolutionVertical, _resolutionHorizontal,
      _refreshRateMin, _refreshRateMax;
  QVector<GPU> _GPUs;
  QVector<Task> tasks;

  void addTask(std::string target, Info info);
  QString convertToGB(QString input);
  QString convertToGHz(QString input);
};