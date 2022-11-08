#include "wmi_api.h"
#include <QDebug>

WMIAPI::WMIAPI(QObject* parent) {
  addTask(Win32_ComputerSystemProduct(std::string("IdentifyingNumber")),
          Info::ProductID);
  addTask(Win32_ComputerSystem(std::string("Name")), Info::MachineID);
  addTask(Win32_OperatingSystem(std::string("Caption")), Info::OS);
  addTask(Win32_OperatingSystem(std::string("Version")), Info::Version);
  addTask(Win32_OperatingSystem(std::string("BuildNumber")), Info::Build);
  addTask(Win32_ComputerSystem(std::string("TotalPhysicalMemory")), Info::Ram);
  addTask(Win32_Processor(std::string("Name")), Info::CpuName);
  addTask(Win32_Processor(std::string("MaxClockSpeed")), Info::CpuClockSpeed);
  addTask(Win32_VideoController(std::string("CurrentHorizontalResolution")),
          Info::ResolutionHorizontal);
  addTask(Win32_VideoController(std::string("CurrentVerticalResolution")),
          Info::ResolutionVertical);
  addTask(Win32_VideoController(std::string("MinRefreshRate")),
          Info::RefreshRateMin);
  addTask(Win32_VideoController(std::string("MaxRefreshRate")),
          Info::RefreshRateMax);
  addTask(Win32_VideoController(std::string("Name")), Info::GpuNames);
  addTask(Win32_VideoController(std::string("DriverVersion")),
          Info::GpuVersions);
  addTask(Win32_VideoController(std::string("AdapterRam")), Info::GpuVram);
}

void WMIAPI::addTask(std::string target, Info type) {
  Task task;
  task.process = new QProcess();
  task.target = QString::fromStdString(target);
  task.type = type;
  tasks.push_back(task);
}

void WMIAPI::runSearch() {
  for (auto& task : tasks) {
    task.process->start(
        "C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe",
        QString(task.target).split(" "));
  }

  QStringList gpuNames, gpuVersions, gpuVrams;

  for (auto& task : tasks) {
    task.process->waitForFinished();

    switch (task.type) {
      case Info::GpuNames:
        gpuNames = QString(task.process->readAllStandardOutput())
                       .remove("-")
                       .remove("Name")
                       .trimmed()
                       .split("\n");
        break;
      case Info::GpuVersions:
        gpuVersions = QString(task.process->readAllStandardOutput())
                          .remove("-")
                          .remove("DriverVersion")
                          .trimmed()
                          .split("\n");
        break;
      case Info::GpuVram:
        gpuVrams = QString(task.process->readAllStandardOutput())
                       .remove("-")
                       .remove("AdapterRam")
                       .trimmed()
                       .split("\n");
    }

    QString result = QString(task.process->readAllStandardOutput())
                         .trimmed()
                         .split("\n")
                         .last()
                         .trimmed()
                         .toLocal8Bit()
                         .constData();

    switch (task.type) {
      case Info::ProductID:
        _productID = result;
        break;
      case Info::MachineID:
        _machineID = result;
        break;
      case Info::OS:
        _os = result;
        break;
      case Info::Version:
        _version = result;
        break;
      case Info::Build:
        _build = result;
        break;
      case Info::Ram:
        _ram = convertToGB(result);
        break;
      case Info::CpuName:
        _cpuName = result;
        break;
      case Info::CpuClockSpeed:
        _cpuClockSpeed = convertToGHz(result);
        break;
      case Info::ResolutionVertical:
        _resolutionVertical = result;
        break;
      case Info::ResolutionHorizontal:
        _resolutionHorizontal = result;
        break;
      case Info::RefreshRateMin:
        _refreshRateMin = result;
        break;
      case Info::RefreshRateMax:
        _refreshRateMax = result;
      default:
        break;
    }
  }

  for (qsizetype i = 0; i < gpuNames.size(); i++) {
    GPU gpu;
    gpu.name = gpuNames[i].toLocal8Bit().constData();
    gpu.driverVersion = gpuVersions[i].toLocal8Bit();
    gpu.VRAM = convertToGB(gpuVrams[i].toLocal8Bit());
    _GPUs.push_back(gpu);
  }
}

QString WMIAPI::productID() {
  return _productID;
}

QString WMIAPI::machineID() {
  return _machineID;
}

QString WMIAPI::os() {
  return _os;
}

QString WMIAPI::version() {
  return _version;
}

QString WMIAPI::build() {
  return _build;
}

QString WMIAPI::RAM() {
  return _ram;
}

QString WMIAPI::CPUName() {
  return _cpuName.split(" ").first(3).join(" ").simplified();
}

QString WMIAPI::CPUClockSpeed() {
  return _cpuClockSpeed;
}

QString WMIAPI::resolution() {
  return QString(_resolutionHorizontal + "x" + _resolutionVertical);
}

QString WMIAPI::refreshRate() {
  return QString(_refreshRateMin + "-" + _refreshRateMax);
}

QVector<WMIAPI::GPU> WMIAPI::GPUs() {
  return _GPUs;
}

QString WMIAPI::convertToGB(QString input) {
  return QString::number(QString(input).toDouble() / 1000000000, 'g', 3)
      .append(" GB");
}

QString WMIAPI::convertToGHz(QString input) {
  return QString::number(QString(input).toDouble() / 1000, 'G', 3)
      .append(" GHz");
}
