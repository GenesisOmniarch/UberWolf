This is a release of a **fork** of UberWolf that adds the ability to choose the output directory for extracted files.
このリリースは UberWolf の**フォーク版**です。展開したファイルの出力先フォルダを指定できる機能を追加しています。

## Downloads / ダウンロード
- `UberWolf-*-x64.zip` – GUI (`UberWolf.exe`), CLI (`UberWolfCli.exe`), README, LICENSE
- `UberWolf.exe` – GUI only / GUI 単体
- `UberWolfCli.exe` – CLI only / CLI 単体

Windows 64-bit. No additional runtime is required. / Windows 64bit 版です。追加のランタイムは不要です。

## New in this fork / フォーク版の追加機能
- **GUI**: *Options* → *Output folder*. Leave empty to extract next to the archive (default).
  **GUI**：*Options* → *Output folder* で出力先を指定。空欄なら従来どおりアーカイブと同じ場所に展開。
- **CLI**: `-d, --output DIR`
  ```
  UberWolfCli.exe "D:\Game\Game.exe" -d "D:\Extracted"
  ```

This fork is not affiliated with the original project. Please do not report issues of this fork to the original repository.
このフォークは元プロジェクトとは無関係です。フォーク版の不具合を元リポジトリへ報告しないでください。
