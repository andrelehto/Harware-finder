#include "HWI.h"

QString HWI::productID() {
  QString result = runProcess("computersystem get model");
  return removeHeader(result);
}

QString HWI::machineID() {
  QString result = runProcess("computersystem get name");
  return removeHeader(result);
}

QString HWI::os() {
  QString result = runProcess("os get caption");
  return removeHeader(result);
}

QString HWI::version() {
  QString result = runProcess("os get version");
  return removeHeader(result);
}

QString HWI::build() {
  QString result = runProcess("os get buildNumber");
  return removeHeader(result);
}

QString HWI::RAM() {
  QString result = runProcess("computersystem get totalphysicalmemory");
  result = removeHeader(result);
  return convertToGB(result);
}

QString HWI::CPUName() {
  QString result = runProcess("cpu get name");
  result = removeHeader(result);
  return result.split(" ").first(3).join(" ").simplified();
}

QString HWI::CPUClockSpeed() {
  QString result = runProcess("cpu get name");
  result = removeHeader(result);
  return result.split(" ").last().simplified();
}

QVector<HWI::GPU> HWI::GPUs() {
  QVector<HWI::GPU> GPUs;

  QString result = runProcess("path win32_VideoController get name").trimmed();
  QStringList names = result.split("\n");
  names.removeFirst();

  result = runProcess("path win32_VideoController get driverVersion").trimmed();
  QStringList versions = result.split("\n");
  versions.removeFirst();

  result = runProcess("path Win32_videocontroller get adapterram").trimmed();
  QStringList VRAMs = result.split("\n");
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
  QString result =
      runProcess("path Win32_videocontroller get CurrentHorizontalResolution")
          .trimmed();
  QString resolution = removeHeader(result);

  resolution.append("x");

  result =
      runProcess("path Win32_videocontroller get CurrentVerticalResolution")
          .trimmed();
  resolution.append(removeHeader(result));

  return resolution;
}

QString HWI::refreshRate() {
  QString result = runProcess("path Win32_videocontroller get MinRefreshRate");
  QString refreshRate = removeHeader(result);

  refreshRate.append("-");

  result = runProcess("path Win32_videocontroller get MaxRefreshRate");
  refreshRate.append(removeHeader(result));

  return refreshRate;
}

QString HWI::runProcess(QString target) {
  QProcess process;
  process.start("wmic", QString(target).split(" "));
  process.waitForFinished();
  return QString(process.readAllStandardOutput());
}

QString HWI::removeHeader(QString string) {
  return string.remove(string.split(" ").first()).simplified();
}

QString HWI::convertToGB(QString string) {
  return QString::number(
             QString(string).simplified().split(" ").last().toDouble() /
                 1000000000,
             'g', 3)
      .append(" GB");
}