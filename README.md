# Qt_MouseEvent

Qt圖像檢視器與編輯工具 / Qt Image Viewer and Editor

## 功能特點 / Features

### 主視窗功能 / Main Window Features

1. **圖片載入** / Image Loading
   - 支援多種圖片格式：PNG、JPG、JPEG、BMP、GIF
   - 自動調整圖片大小以適應視窗

2. **滑鼠位置追蹤** / Mouse Position Tracking
   - 即時顯示滑鼠游標座標
   - 顯示滑鼠事件（左鍵、右鍵、中鍵、雙擊、釋放）

3. **灰階值顯示** / Grayscale Value Display
   - 當游標移動到圖片上時，即時顯示該位置的灰階值

4. **區域選取放大** / Region Selection and Zoom
   - 在圖片上按住左鍵拖曳以選取要放大的區域
   - 可自訂放大倍率（0.5x 到 10x）
   - 選取完成後會彈出新視窗顯示放大的圖片

### 放大視窗功能 / Zoom Window Features

1. **另存新檔** / Save As
   - 支援將編輯後的圖片儲存為 PNG、JPG 或 BMP 格式
   - 快捷鍵：Ctrl+S（或 Cmd+S）

2. **畫筆工具** / Brush Tool
   - 可在放大的圖片上繪圖
   - 可選擇畫筆顏色
   - 可調整畫筆大小（1-100 像素）
   - 點擊「畫筆工具」按鈕或選單啟用/停用

## 使用方法 / Usage

### 載入圖片 / Load an Image

1. 啟動程式
2. 點擊選單「檔案」→「開啟圖片」
3. 選擇要檢視的圖片檔案

### 檢視灰階值 / View Grayscale Value

- 載入圖片後，將滑鼠移動到圖片上
- 狀態列會顯示游標位置的灰階值（0-255）

### 放大圖片區域 / Zoom into Image Region

1. 載入圖片
2. （選擇性）調整放大倍率：選單「檢視」→「設定放大倍率」
3. 在圖片上按住左鍵並拖曳以選取要放大的區域
4. 釋放滑鼠後，會彈出新視窗顯示放大的圖片

### 使用畫筆編輯圖片 / Edit Image with Brush

在放大視窗中：

1. 點擊選單「工具」→「畫筆工具」或工具列的畫筆按鈕
2. （選擇性）選擇畫筆顏色：「工具」→「選擇畫筆顏色」
3. （選擇性）設定畫筆大小：「工具」→「設定畫筆大小」
4. 按住左鍵在圖片上拖曳以繪圖
5. 完成後點擊「檔案」→「另存新檔」儲存編輯後的圖片

## 建置方法 / Build Instructions

### 需求 / Requirements

- Qt 6.x 或更高版本
- C++17 編譯器

### 建置步驟 / Build Steps

使用 qmake：
```bash
qmake Qt_MouseEvent.pro
make
```

或使用 Qt Creator：
1. 開啟 `Qt_MouseEvent.pro` 專案檔
2. 點擊「建置」按鈕
3. 點擊「執行」按鈕

## 技術說明 / Technical Details

### 檔案結構 / File Structure

- `main.cpp` - 程式入口
- `mouseevent.h/cpp` - 主視窗類別，處理滑鼠事件和圖片顯示
- `zoomwindow.h/cpp` - 放大視窗類別，提供圖片編輯功能

### 主要類別 / Main Classes

#### MouseEvent
- 繼承自 `QMainWindow`
- 負責圖片載入、顯示和區域選取
- 顯示滑鼠位置和灰階值

#### ZoomWindow
- 繼承自 `QMainWindow`
- 顯示放大後的圖片
- 提供畫筆工具和另存新檔功能

## 授權 / License

本專案採用開源授權。

