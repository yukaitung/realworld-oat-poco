# ![RealWorld Example App](logo.png)

[廣東話 (Cantonese) (盡量翻譯)](#廣東話) 

> ### C++, POCO, Oat++ codebase containing real-world examples (CRUD, auth, MVC patterns, etc.) that adheres to the [RealWorld](https://github.com/gothinkster/realworld) specification and API.

This codebase was created to demonstrate a fully-fledged full-stack application built with **C++, POCO, Oat++**, including CRUD operations, authentication, routing, pagination, and more.

For more information on how this works with other frontends/backends, head over to the [RealWorld](https://github.com/gothinkster/realworld) repository.

# Try To Use British English

All variable names, comments, function names, etc., should use British English. American English is used to satisfy the API requirements.

# To-Do List

1. Integrate Swagger-UI.
2. Integrate with GitLab CI/CD.
3. Write tests.
4. Optimize performance / Remove duplicated code.

# How it works

The Oat++ framework is used for web development, and POCO is utilized to connect to the MySQL database.

# Getting started

This project requires a C++ compiler, CMake 2.23, and [Conan 2.0](https://conan.io/).

To build the project on macOS:

```bash
  conan install . --output-folder=build --build=missing
  cd build
  cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j $(sysctl -n hw.ncpu)
```

# 廣東話
# ![RealWorld 示範應用程式](logo.png)

> ### C++, POCO, Oat++ 現實應用程式碼(CRUD、登入、MVC pattern、其他) 符合 [RealWorld](https://github.com/gothinkster/realworld) 規格及 API.

這個程式碼是用來展示 **C++、POCO、Oat++** 建立的完整應用程式，包括 CRUD、登入、routing、分頁等功能。

如需更多關於如何與其他 Frontend / Backend 配合使用的資訊，請參考 [RealWorld](https://github.com/gothinkster/realworld) GitHub repository。

# 使用英式英文

所有變數名稱、註解、函數名稱等都應該使用英式英文。美式英文只是為了符合 API 規格。

# 待辦事項

1. 整合 Swagger-UI。
2. 與 GitLab CI/CD 整合。
3. 撰寫測試。
4. 優化效能 / 刪除重複程式碼。

# 如何運作

Oat++ 是一個網頁框架，POCO 用於連接 MySQL 資料庫。

# 入門指南

此專案需要 C++ 編譯器、CMake 2.23 和 [Conan 2.0](https://conan.io/)。

在 macOS 上建立：

```bash
  conan install . --output-folder=build --build=missing
  cd build
  cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
  cmake --build . -j $(sysctl -n hw.ncpu)
```
