# UIBaseCore - 基础UI组件库

## 概述

UIBaseCore是一个纯UI组件库，提供可复用的Qt UI控件和组件。

## 设计原则

1. **纯UI组件**：只包含UI相关的代码，不涉及业务逻辑
2. **零依赖**：不依赖任何业务层模块（第0层、第1层、第2层、第3层、第4层）
3. **可复用**：提供通用的UI组件，可以被其他UI模块复用
4. **跨平台**：支持Windows和Linux平台

## 包含的组件

### 自定义控件

1. **hgqlabel.h/cpp** - 自定义标签控件
   - 支持图片和文字显示
   - 支持点击事件

2. **labelwithimg.h/cpp** - 带图片的标签控件
   - 支持图片和文字组合显示
   - 支持样式定制

3. **QRoundProgressBar.h/cpp** - 圆形进度条
   - 圆形进度显示
   - 支持自定义颜色和样式

4. **DragDialog.h/cpp** - 可拖拽对话框
   - 支持拖拽功能
   - 支持无边框窗口

### 图形视图组件

5. **mygraphicsview.h/cpp** - 自定义图形视图
   - 支持图形显示和操作
   - 支持缩放和平移

### 图形图元组件

6. **rectitem.h/cpp** - 矩形图元
   - 支持矩形的绘制和编辑
   - 支持拖拽和缩放

7. **rectellipseitem.h/cpp** - 椭圆图元
   - 支持椭圆的绘制和编辑
   - 支持拖拽和缩放

## 依赖关系

UIBaseCore只依赖：
- Qt6::Gui
- Qt6::Core
- Qt6::Widgets
- Qt6::Charts
- Qt6::PrintSupport

UIBaseCore不依赖：
- 第0层（Base前缀）
- 第1层（Svc前缀）
- 第2层（Hw前缀）
- 第3层（Core前缀）
- 第4层（If前缀）

## 使用示例

```cpp
#include "hgqlabel.h"

// 创建自定义标签
HGQLabel* label = new HGQLabel(true, "path/to/image.png");
label->setText("Hello World");
```

## 注意事项

1. 不要在UIBaseCore中添加业务逻辑代码
2. 不要在UIBaseCore中依赖任何业务层模块
3. 保持组件的通用性和可复用性
4. 遵循Qt的编码规范

## 版本历史

- v1.0.0 - 初始版本，包含基础UI组件
