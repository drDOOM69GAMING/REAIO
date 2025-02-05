# REInstaller

A Qt-based **Resident Evil (Biohazard) Mod Installer** that simplifies the process of installing and managing mods for Resident Evil 1, 2, and 3. It automates file extraction, organization, and installation while providing a user-friendly interface.

## Features

- **Graphical User Interface (GUI)** built with Qt
- **Supports Resident Evil 1, 2, and 3** mod installations
- **Automatic mod downloads** from predefined URLs
- **Dynamic UI updates** based on selected game
- **Mod installation modes**: Auto, AMD, and Nvidia
- **Background image updates** per game selection
- **Real-time logging** of installation process
- **Multi-threading support** via `QtConcurrent`
- **ZIP extraction** using `QuaZip`
- **Help menu** with installation instructions

## Installation

1. **Install Dependencies**
   ```bash
   sudo apt-get install qt5-default qttools5-dev-tools
   ```
   or (for Windows, use Qt Installer to get Qt Creator and required libraries)

2. **Clone the Repository**
   ```bash
   git clone https://github.com/drDOOM69GAMING/REAIO.git
   cd REAIO
   ```

3. **Build the Application**
   ```bash
   qmake && make  # Linux/macOS
   qmake && nmake # Windows (if using MSVC)
   ```

4. **Run the Application**
   ```bash
   ./REAIO  # Linux/macOS
   REAIO.exe # Windows
   ```

## Usage

1. Launch `REAIO`.
2. Select the **Resident Evil** game you want to mod.
3. Choose a mod from the dropdown menu.
4. Click `Download Selected Mod` to fetch the mod.
5. Select an installation mode (Auto, AMD, or Nvidia) if applicable.
6. The mod will be extracted and installed automatically.

## Requirements

- **Japanese versions** of Resident Evil games.
- **Game folders must be on Desktop**:
  - RE1: `BIOHAZARD Mediakite`
  - RE2: `biohazard-2-apan-source-next`
  - RE3: `Bio Hazard 3 (SOURCENEXT)`
- **Mod ZIP files** should be placed in `Desktop` or `Downloads`.

## Supported Mods

| Mod Name             | Download URL |
|----------------------|-------------|
| Biohazard Mod       | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Biohazard_mod.zip) |
| Bio2 Mod            | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Bio2_mod.zip) |
| Bio3 Mod            | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/Bio3_mod.zip) |
| dgVoodoo AMD Fix    | [Link](https://github.com/TheOtherGuy66-source/Resident_Evil_Python_Builder_kit/releases/download/amd/dgVoodoo_AMD_fix.zip) |

## Contributing

Contributions are welcome! If you’d like to improve this project:
1. **Fork** the repository.
2. Create a **new branch** for your feature/fix.
3. **Commit** your changes with a descriptive message.
4. Open a **Pull Request**.

## Credits

- **TeamX** - Original modding concepts
- **RESHDP & Gemini** - Seamless HD Project support
- **Qt Framework** - GUI & multi-threading

## Troubleshooting

If you encounter issues:
- Ensure you have the correct game versions.
- Check the **log area** for detailed error messages.
- Verify **Qt plugins** are correctly loaded.
- Make sure required dependencies (`Qt5, QuaZip`) are installed.
