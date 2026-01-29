# CoreUserAudit 模块使用指南

## 概述

CoreUserAudit 模块是从 UIBaseData 中抽离出来的用户管理与权限管理功能，提供了统一的用户和权限信息管理接口。

## 架构设计

```
UI 模块 → CoreUserAudit (实现)
```

### 架构说明

**CoreUserAudit (实现层)** - `modules/CoreUserAudit/`
- 包含用户和权限管理的完整实现
- 包含 UsersInfoCache 和 AuthorityInfoCache
- 直接操作数据库（RWDb）
- 自动记录审计日志
- UI 模块直接使用，不通过 SvcFactory

### 依赖关系

```
UI 模块
  ↓ 直接使用
CoreUserAudit
  ↓ 依赖
IfLog (接口)
```

**关键点：**
- CoreUserAudit 是业务模块，不通过 SvcFactory 创建
- UI 模块直接包含 CoreUserAudit.h 并创建实例
- SvcFactory 只包含基础服务接口（如 IBaseCommon, ITime, IDatabaseManager 等）

## 使用方式

### 1. 包含头文件

```cpp
#include "CoreUserAudit.h"
```

### 2. 创建实例

```cpp
// 直接创建实例
HGMACHINE::CoreUserAudit userAudit;

// 初始化
userAudit.initialize();
```

### 3. 用户管理

```cpp
// 获取所有用户信息
auto users = userAudit.getUsersInfo();

// 获取特定用户信息
auto userInfo = userAudit.getUserInfo("user001");

// 添加新用户
std::map<std::string, std::string> newUserInfo;
newUserInfo["UserAccount"] = "newuser";
newUserInfo["UserName"] = "New User";
newUserInfo["Authority"] = "User";
// ... 其他字段
userAudit.addUserRecord(newUserInfo);

// 删除用户
userAudit.deleteUserInfo("user001");

// 修改用户信息
int index = userAudit.getUserFieldIndex("user001");
if (index >= 0) {
    userAudit.setUserField(index, "UserName", "Updated Name");
}
```

### 4. 权限管理

```cpp
// 获取所有权限信息
auto authorities = userAudit.getAuthorityInfo();

// 获取用户权限
auto userAuthority = userAudit.getUserAuthority("user001");

// 获取权限详情
auto authorityDetail = userAudit.getAuthorityDetail("Admin");

// 权限编辑
userAudit.beginAuthorityEdit();
// ... 修改权限
userAudit.commitAuthorityEdit();  // 提交修改
// 或
userAudit.rollbackAuthorityEdit();  // 回滚修改
```

### 5. 保存数据

```cpp
// 保存用户信息
userAudit.saveUsersInfo();

// 保存权限信息
userAudit.saveUserGroupInfo();
```

### 6. 关闭实例

```cpp
userAudit.shutdown();
```

## 迁移指南

### 从 GlobalSingleton 迁移到 CoreUserAudit

#### 原代码：
```cpp
#include "globalsingleton.h"

// 获取用户信息
auto users = GlobalSingleton::instance().getUsersInfo();
auto userInfo = GlobalSingleton::instance().getUserInfo(index);

// 删除用户
GlobalSingleton::instance().deleteUserInfo("user001");

// 获取权限信息
auto authorities = GlobalSingleton::instance().getAuthorityInfo();
```

#### 新代码：
```cpp
#include "CoreUserAudit.h"

// 创建实例
HGMACHINE::CoreUserAudit userAudit;
userAudit.initialize();

// 获取用户信息
auto users = userAudit.getUsersInfo();
auto userInfo = userAudit.getUserInfo(index);

// 删除用户
userAudit.deleteUserInfo("user001");

// 获取权限信息
auto authorities = userAudit.getAuthorityInfo();

// 关闭实例
userAudit.shutdown();
```

## 优势

1. **模块化**：用户和权限管理功能独立成模块，便于维护和扩展
2. **业务封装**：业务逻辑完全封装在 CoreUserAudit 中
3. **自动审计**：关键操作（删除用户、提交权限编辑）自动记录审计日志
4. **线程安全**：内部使用互斥锁保证线程安全
5. **直接使用**：UI 模块直接创建和使用，不通过 SvcFactory
6. **职责清晰**：SvcFactory 只管理基础服务，CoreUserAudit 管理业务逻辑

## 注意事项

1. 使用前必须调用 `initialize()` 初始化
2. 使用完毕后建议调用 `shutdown()` 保存数据
3. 权限编辑需要先调用 `beginAuthorityEdit()`，然后调用 `commitAuthorityEdit()` 提交或 `rollbackAuthorityEdit()` 回滚
4. 删除用户和提交权限编辑会自动记录审计日志
5. CoreUserAudit 是业务模块，不通过 SvcFactory 创建
6. SvcFactory 只包含基础服务接口（如 IBaseCommon, ITime, IDatabaseManager 等）

## 模块文件结构

```
modules/CoreUserAudit/
├── inc/
│   ├── CoreUserAudit.h           # 核心用户审计类
│   ├── UsersInfoCache.h          # 用户信息缓存
│   └── AuthorityInfoCache.h      # 权限信息缓存
├── src/
│   ├── CoreUserAudit.cpp
│   ├── UsersInfoCache.cpp
│   └── AuthorityInfoCache.cpp
└── CMakeLists.txt
```

## 与 SvcFactory 的区别

### SvcFactory 中的服务（基础服务）
- IBaseCommon - 基础通用服务
- IBaseConfig - 配置服务
- ISvcSave - 保存服务
- ISvcLog - 日志服务
- ISvcFrame - 框架服务
- ITime - 时间服务
- IDatabaseManager - 数据库管理服务

这些是**基础服务**，不涉及具体业务逻辑，适合通过 SvcFactory 统一管理。

### CoreUserAudit（业务模块）
- 用户管理
- 权限管理

这些是**业务模块**，涉及具体业务逻辑，不适合通过 SvcFactory 管理，UI 模块直接使用。
