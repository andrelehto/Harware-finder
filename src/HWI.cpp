#include "HWI.h"
#include <QDebug>

QString HWI::productID() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_ComputerSystemProduct | "
      "Select-Object IdentifyingNumber");
  return result.trimmed().split("\n").last();
}

QString HWI::machineID() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_ComputerSystem | "
      "Select-Object Name");
  return result.trimmed().split("\n").last();
}

QString HWI::os() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_OperatingSystem | "
      "Select-Object Caption");
  return result.trimmed().split("\n").last();
}

QString HWI::version() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_OperatingSystem | "
      "Select-Object Version");
  return result.trimmed().split("\n").last();
}

QString HWI::build() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_OperatingSystem | "
      "Select-Object BuildNumber");
  return result.trimmed().split("\n").last();
}

QString HWI::RAM() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_ComputerSystem | "
      "Select-Object TotalPhysicalMemory");
  result = result.trimmed().split("\n").last();
  return convertToGB(result);
}

QString HWI::CPUName() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_Processor | "
      "Select-Object Name");
  result = result.remove("-").trimmed().split("\n").last();
  return result.split(" ").first(3).join(" ").simplified();
}

QString HWI::CPUClockSpeed() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_Processor | "
      "Select-Object Name");
  return result.split(" ").last().simplified();
}

QVector<HWI::GPU> HWI::GPUs() {
  QVector<HWI::GPU> GPUs;

  QString result = runProcess(
                       "Get-CimInstance -ClassName Win32_VideoController | "
                       "Select-Object Name")
                       .trimmed();
  QStringList names = result.remove("-").trimmed().split("\n");
  names.removeFirst();
  names.removeFirst();

  result = runProcess(
               "Get-CimInstance -ClassName Win32_VideoController | "
               "Select-Object DriverVersion")
               .trimmed();
  QStringList versions = result.remove("-").trimmed().split("\n");
  versions.removeFirst();
  versions.removeFirst();

  result = runProcess(
               "Get-CimInstance -ClassName Win32_VideoController | "
               "Select-Object AdapterRam")
               .trimmed();
  QStringList VRAMs = result.remove("-").trimmed().split("\n");
  VRAMs.removeFirst();
  VRAMs.removeFirst();

  for (int i = 0; i < names.size(); i++) {
    GPU gpu;
    gpu.name = names[i].trimmed();
    gpu.driverVersion = versions[i].trimmed();
    gpu.VRAM = convertToGB(VRAMs[i]).trimmed();
    GPUs.push_back(gpu);
  }

  return GPUs;
}

QString HWI::resolution() {
  QString result = runProcess(
                       "Get-CimInstance -ClassName Win32_VideoController | "
                       "Select-Object CurrentHorizontalResolution")
                       .trimmed();
  QString resolution = result.trimmed().split("\n").last().simplified();

  resolution.append("x");

  result = runProcess(
               "Get-CimInstance -ClassName Win32_VideoController | "
               "Select-Object CurrentVerticalResolution")
               .trimmed();
  resolution.append(result.trimmed().split("\n").last().simplified());

  return resolution;
}

QString HWI::refreshRate() {
  QString result = runProcess(
      "Get-CimInstance -ClassName Win32_VideoController | Select-Object "
      "MinRefreshRate");
  QString refreshRate = result.trimmed().split("\n").last().simplified();

  refreshRate.append("-");

  result = runProcess(
      "Get-CimInstance -ClassName Win32_VideoController | Select-Object "
      "MaxRefreshRate");
  refreshRate.append(result.trimmed().split("\n").last().simplified());

  return refreshRate;
}

QString HWI::runProcess(QString target) {
  QProcess process;
  process.start("C:/Windows/system32/WindowsPowerShell/v1.0/powershell.exe",
                QString(target).split(" "));
  process.waitForFinished();
  return QString(process.readAllStandardOutput());
}

QString HWI::removeHeader(QString string) {
  return string.remove(string.split(" ").first()).simplified().remove("-");
}

QString HWI::convertToGB(QString string) {
  return QString::number(
             QString(string).simplified().split(" ").last().toDouble() /
                 1000000000,
             'g', 3)
      .append(" GB");
}