> [!NOTE]
> **This is a fork of [Sinflower/UberWolf](https://github.com/Sinflower/UberWolf).**
> It adds the ability to **choose the output directory** for extracted files (GUI and CLI).
> By default, files are still extracted next to each archive, exactly like the original.
>
> **このリポジトリは [Sinflower/UberWolf](https://github.com/Sinflower/UberWolf) のフォークです。**
> 展開したファイルの**出力先フォルダを指定できる機能**を追加しています（GUI・CLI 両対応）。
> 出力先を指定しない場合は、従来どおりアーカイブと同じ場所に展開されます。

# UberWolf GUI
 Fully functional, GUI-based Wolf RPG decrypter with support for Wolf RPG Pro.

 ![](doc/UI.png)

### Features

 - GUI application
 - Simple usage, based on drag and drop
 - Support for all possible file extensions
 - Automatic decryption key detection
 - Detection of the Pro Editor ***Protection Key*** if present

### Supported File Extensions
 - .wolf
 - .data
 - .pak
 - .bin
 - .assets
 - .content
 - .res
 - .resource

## Usage - Full Game Processing
- Drag and drop ***Game.exe*** or ***GamePro.exe*** onto the dedicated area or select it using the ***Select Game*** button.
- Press Process to unpack all archive files and, if present, detect the ***Protection Key***.

## Usage - Extract One or More Archives
 Drag and drop the archives onto the designated area and wait until the files are extracted.

## Output Directory (fork feature)
 By default, every archive is extracted into a folder next to it (e.g., `Data/BasicData.wolf` → `Data/BasicData/`).
 To extract somewhere else, open ***Options*** and enter a folder in ***Output folder*** (or pick one with the ***...*** button).
 The setting is saved and applies to both ***Process*** and drag and drop. Leave it empty to restore the default behavior.

 With an output folder set, `Data/BasicData.wolf` is extracted to `<Output folder>/BasicData/`.
 The protection key detection, ***Unprotect Pro files*** and ***Decrypt WolfX files*** options also work on the extracted files in the output folder.

 出力先を変更するには、***Options*** を開き ***Output folder*** にフォルダを入力（または ***...*** ボタンで選択）してください。空欄にすると従来どおりアーカイブと同じ場所に展開されます。

----

# UberWolfCli
 Simple command line interface to decrypt Wolf RPG games.

## Usage
 The usage of UberWolfCli is very similar to the GUI application:
 - Drag and drop ***Game.exe*** or ***GamePro.exe*** onto the executable to automatically detect the data folder and decrypt all files.
 - Drag and drop a ***folder*** folder onto the executable to extract all supported wolf files within it.
 - Drag and drop one or more supported files (e.g., ***.wolf***) onto the executable to extract them all.
 - Call it from the command line like this:
```bash
# Full auto-detect
UberWolfCli.exe "D:\Path to Game\Game.exe"
# Using a folder, e.g., Data
UberWolfCli.exe "D:\Path to Game\Data"
# Unpacking a single file
UberWolfCli.exe "D:\Path to Game\Data\BasicData.wolf"
```

### Output Directory (fork feature)
 Use `-d` / `--output` to extract into a specific directory instead of next to the archives.
 The directory is created if it does not exist.
```bash
# Full auto-detect, extract everything to D:\Extracted
UberWolfCli.exe "D:\Path to Game\Game.exe" -d "D:\Extracted"
# Unpacking a single file into another folder
UberWolfCli.exe "D:\Path to Game\Data\BasicData.wolf" --output "D:\Extracted"
```

### Options
| Option | Description |
| --- | --- |
| `-o, --override` | Override existing files |
| `-u, --unprotect` | Unprotect Pro files |
| `-x, --wolfx` | Decrypt WolfX files if present |
| `-d, --output DIR` | Output directory for extracted files (default: next to each archive) |
| `-p, --pack VER_IDX` | Pack the data folder using the given encryption |

----

# Localization

 Please look at the [Localization Guide](doc/localization.md) for information on translating the application into your language.

### Credits
- Japanese (MTL) - Brothers3398
- Korean (MTL) - Brothers3398
- Chinese - Raylern
- Traditional Chinese - love80312
- Russian - SavDor