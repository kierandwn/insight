# Dependency Install Procedures 

Qwt source available [here](https://sourceforge.net/projects/qwt/files/qwt/6.1.5/qwt-6.1.5.zip/download).  

## Windows 10
_This guide assumes that Microsoft Visual Studio is already installed._

### Qt 5.15.1

Qt install instructions are [here](https://doc.qt.io/qt-5/gettingstarted.html).

Personally, I used the online installer, selected `Qt v5.15.1` with compiler `MSVC2019 64bit`. Installed to `C:\Qt`.

### Qwt 6.1.5

- Download Qwt source .zip.
- Extract to C:\Qwt-6.1.5
- Open Qt 5.15.1 (MSVC2019 64-bit) command line, and:
  - execute `C:\Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvars64.bat`
  - navigate to C:\Qwt-6.1.5
  - run 

```
qmake qwt.pro
nmake
namke install
```

### Configure Qwt 6.1.5 plugins to work with QtDesigner

- Modify the following environment variables (Control Panel->Set Environment Variables):
 - `QWT_ROOT` : `C:\Qwt-6.1.5`
 - Add `C:\Qwt-6.1.5` to `PATH`
- Copy `C:\qwt-6.1.5\designer\plugins\designer\qwt_designer_plugin.dll` to `C:\Qt\Tools\QtCreator\bin\plugins\designer`

## Linux (Debian)

Install Qt5:

```
sudo apt-get install qtcreator
sudo apt-get install qt5-default
sudo apt-get install qt5-doc qtbase5-doc-html
sudo apt-get install qttools5-dev
```
