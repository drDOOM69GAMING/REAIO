# REAIO - Resident Evil AIO Mod Installer

A **Qt-based Resident Evil (Biohazard) Mod Installer** designed to simplify mod installation and management for Resident Evil 1, 2, and 3. This tool automates file extraction, organization, and installation while providing a user-friendly graphical interface.

---

## Features

- **Graphical User Interface (GUI)** built with Qt  
- **Supports Resident Evil 1, 2, and 3** mod installations  
- **Automatic mod downloads** from predefined URLs  
- **Dynamic UI updates** based on selected game  
- **Mod installation modes**: Auto, AMD, and Nvidia  
- **Background image updates** per game selection  
- **Real-time logging** of the installation process  
- **Multi-threading support** via `QtConcurrent`  
- **ZIP extraction** using `QuaZip`  
- **Help menu** with installation instructions  

---

## 🛠 Installation

### Prerequisites

1. **MSYS2 (64-bit)**: Install from [msys2.org](https://www.msys2.org/).
2. **Ensure System Path Configuration:** Add `C:\msys64\mingw64\bin` to your system PATH.

### Step 1: Install Dependencies

Run the following commands in the MSYS2 terminal:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-gcc \
          mingw-w64-x86_64-pkg-config \
          mingw-w64-x86_64-qt5 \
          mingw-w64-x86_64-quazip \
          mingw-w64-x86_64-cmake \
          mingw-w64-x86_64-make \
          mingw-w64-x86_64-libtool \
          mingw-w64-x86_64-zlib \
          mingw-w64-x86_64-openssl
```

### Step 2: Clone the Repository

```bash
git clone https://github.com/drDOOM69GAMING/REAIO.git
cd REAIO
```

### Step 3: Compile the Application

1. Generate the meta-object code:

   ```bash
   moc REAIO.cpp -o REAIO.moc
   ```

2. Compile the project:

   ```bash
   g++ -I/mingw64/include/qt5 -I/mingw64/include/QuaZip-Qt5-1.4/quazip \
       -L/mingw64/lib \
       -o REAIO.exe REAIO.cpp \
       $(pkg-config --cflags --libs Qt5Widgets Qt5Gui Qt5Core Qt5Network Qt5Concurrent) \
       -lquazip1-qt5 -lz
   ```

### Step 4: Deploy Dependencies

#### Automatic Deployment (Recommended)

```bash
windeployqt REAIO.exe
```

This command copies all required Qt DLLs and plugins automatically.

#### Manual Deployment (If Issues Occur)

Copy the following from `C:\msys64\mingw64\bin` to your project folder:

```
Qt5Core.dll, Qt5Gui.dll, Qt5Widgets.dll, Qt5Network.dll, Qt5Concurrent.dll, 
libquazip1-qt5.dll, zlib1.dll, libwinpthread-1.dll, libstdc++-6.dll, libgcc_s_seh-1.dll
```

Create subfolders `plugins/imageformats` and copy:

```
qwindows.dll (from mingw64/plugins/platforms)
*.dll (from mingw64/plugins/imageformats)
```

---

## Directory Structure

Ensure your project folder contains the following:

```
REAIO.exe
REAIO.cpp
RE1.png, RE2.png, RE3.png
bearer/
iconengines/
platforms/
plugins/imageformats/
styles/
+ All DLLs listed in the deployment step
```

---

## Usage

1. Launch `REAIO.exe`.
2. Select the **Resident Evil** game you want to mod.
3. Choose a mod from the dropdown menu.
4. Click `Download Selected Mod` to fetch the mod.
5. Select an installation mode (Auto, AMD, or Nvidia) if applicable.
6. The mod will be extracted and installed automatically.

---

## Requirements

- **Japanese versions** of Resident Evil games.
- **Game folders must be on Desktop**:
  - RE1: `BIOHAZARD Mediakite`
  - RE2: `biohazard-2-apan-source-next`
  - RE3: `Bio Hazard 3 (SOURCENEXT)`
- **Mod ZIP files** should be placed in `Desktop` or `Downloads`.

---

## Supported Mods

| Mod Name             | Download URL |
|----------------------|--------------|
| Biohazard Mod         | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Biohazard_mod.zip) |
| Bio2 Mod              | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Bio2_mod.zip) |
| Bio3 Mod              | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Bio3_mod.zip) |
| dgVoodoo AMD Fix      | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/dgVoodoo_AMD_fix.zip) |

---

## Contributing

Contributions are welcome! If you’d like to improve this project:

1. **Fork** the repository.  
2. Create a **new branch** for your feature/fix.  
3. **Commit** your changes with a descriptive message.  
4. Open a **Pull Request**.  

---

## 🛠 Troubleshooting

- Ensure you have the correct game versions.
- Check the **log area** for detailed error messages.
- Verify **Qt plugins** are correctly loaded.
- Make sure required dependencies (`Qt5`, `QuaZip`) are installed.

---

## Credits

- **TeamX** - Original modding concepts  
- **RESHDP & Gemini** - Seamless HD Project support  
- **Qt Framework** - GUI & multi-threading  

---

## 📝 License

This project is licensed under the **MIT License**. You are free to use, modify, and distribute this software, provided you include the original copyright notice and license.

### MIT License

```plaintext
Copyright (c) 2025 drDOOM69GAMING

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

### Screenshots

![Screenshot 1](https://github.com/user-attachments/assets/f22f001e-9ec8-488a-924f-48fcb183b8f3)  
![Screenshot 2](https://github.com/user-attachments/assets/8f8e637d-eb59-4787-b345-6a7690db440d)  
![Screenshot 3](https://github.com/user-attachments/assets/a5e2071c-7fe8-4534-a050-fec4db83b883)
