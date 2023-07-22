# ![RealWorld Example App](img/logo.png)

![GitLab Pipeline](https://gitlab.com/yukaitung/realworld-oat-poco/badges/main/pipeline.svg)

[廣東話 (Cantonese)](#廣東話) 

[Coming from GitHub? Press here to redirect to GitLab Repository](https://gitlab.com/yukaitung/realworld-oat-poco)

> ### C++, Oat++, POCO codebase containing real-world examples (CRUD, auth, MVCS patterns, etc.) that adheres to the [RealWorld](https://github.com/gothinkster/realworld) specification and API.

This codebase was created to demonstrate a fully-fledged full-stack application built with **C++, Oat++, POCO**, including CRUD operations, authentication, routing, pagination, and more.

For more information on how this works with other frontends/backends, head over to the [RealWorld](https://github.com/gothinkster/realworld) repository.

### Links

*   [GitLab Repository](https://gitlab.com/yukaitung/realworld-oat-poco)
*   [GitHub Repository](https://github.com/yukaitung/realworld-oat-poco)
*   [Project Docker Repository](https://hub.docker.com/r/yukaitung/realworld-oat-poco)

# Try To Use British English

All API endpoints, parameters and source code, should use British English. Please change `favorite` into `favourite` when evaluating the project. I have an the UK verson of the [Postman Collection](test/Conduit.postman_collection_uk.json).

# How it works
### Software

This project uses Oat++ and POCO. Below are their functions.

Oat++:

* Hosting a RESTful API
* Authenticating a user
* Validating user input
* Running API unit tests
* Integrating with Swagger UI

POCO:

* Generating and validating JWT Token
* Encoding / decoding the [URI format](https://docs.pocoproject.org/current/Poco.URI.html#20774)
* Connecting to MySQL server
* Handling of timezone by creating a timezone aware DateTime object

### MVCS Pattern

This project follows the MVCS pattern [(Example)](https://github.com/oatpp/example-crud) [(What is MVCS)](https://stackoverflow.com/questions/5702391/mvcs-model-view-controller-service). Below is how the project interprets the MVCS.

* The Controller is providing an endpoint and calling a corresponding Service.
* The Service is retrieving data from one or more Models and processing the data.
* The Model is actually operating CRUD data in the database.

### Unicode Support

Most operating systems should support Unicode out-of-the-box (except on Microsoft Windows? Not sure). You can insert data using any Unicode-supported language. The below screenshot is inserting some Cantonese data in macOS. Note that the slug is encoded into the [URI format](https://docs.pocoproject.org/current/Poco.URI.html#20774).

![Unicode Test](img/utf8.png)

# Getting started

### Prerequisite

This project requires a C++ compiler, CMake 2.23, [Conan 2.0](https://conan.io/), MySQL, and pip. In macOS, [Homebrew](https://docs.brew.sh/Installation) can be an alternative to pip.

In Ubuntu, you can install the dependency by following command. Note that CMake is installed via pip because the version may be too old in Ubuntu.

```bash
sudo apt install -qy --no-install-recommends g++ make pip perl libmysqlclient-dev
sudo pip install conan cmake
sudo ln -s /usr/local/bin/cmake /usr/bin/cmake
```

In macOS, you should have Xcode Command Line Tools installed. Then you can install conan via Homebrew.

```bash
brew install conan
```

Then, run following command to [genetate profile for conan](https://docs.conan.io/2/reference/commands/profile.html)

```bash
conan profile detect --force
```

### Build

To build the project on Linux / macOS, navigate to the project folder. Run the following commands, replace the `/usr/include/mysql` if your MySQL is installed in another directory.

```bash
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_CXX_FLAGS=-isystem\ /usr/include/mysql -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . -j`nproc` # Linux
cmake --build . -j$(sysctl -n hw.ncpu) # macOS
```

### Run

First you should create the SQL schema. The comments below indicate the creation of the database name `Realworld`. Note that this is unsafe and intended only for testing purposes. Refer to [realworld.sql](sql/realworld.sql) fo deteiled schema.

```bash
sudo mysql -e "CREATE USER 'admin'@'127.0.0.1' IDENTIFIED BY '123456';"
sudo mysql -e "GRANT ALL PRIVILEGES ON *.* TO 'admin'@'127.0.0.1';"
sudo mysql -e "CREATE DATABASE Realworld;"
sudo mysql RealworldTest < sql/realworld.sql
```

Then you need to set up environment variables before executing the program.

| Variables Name    | Description |
| ----------------- | ----------- |
| REALWORLD_DB_HOST | MySQL Server IP Address |
| REALWORLD_DB_PORT | MySQL Server Port |
| REALWORLD_DB_NAME | Database Name |
| REALWORLD_DB_USER | MySQL Server Username |
| REALWORLD_DB_PASSWORD | MySQL Server Password |
| REALWORLD_SIGNER_SECRET | JWT Singer Secret |
| REALWORLD_CORS_ORIGIN | CORS Access-Control-Allow-Origin |

Example:

```bash
export REALWORLD_DB_HOST=127.0.0.1
export REALWORLD_DB_PORT=3306
export REALWORLD_DB_NAME=Realworld
export REALWORLD_DB_USER=admin
export REALWORLD_DB_PASSWORD=123456
export REALWORLD_SIGNER_SECRET=some_random_words
cd bin # The executable is in the bin folder
./Realworld
```

Below are the endpoints to evaluating the project.

* http://localhost:8000/
* http://localhost:8000/swagger/ui

You can use the [Postman Collection](test/Conduit.postman_collection_uk.json) to test the project. Additionally, this project is integrated with Swagger UI. Navigate to [http://localhost:8000/swagger/ui](http://localhost:8000/swagger/ui) to try it on the browser.

### Test

If you would like to run the unit test, first you should create another database. Then, set up environment variables as follows.

```bash
export REALWORLD_TEST_DB_HOST=127.0.0.1
export REALWORLD_TEST_DB_PORT=3306
export REALWORLD_TEST_DB_NAME=RealworldTest
export REALWORLD_TEST_DB_USER=admin
export REALWORLD_TEST_DB_PASSWORD=123456
export REALWORLD_TEST_SIGNER_SECRET=some_random_words
./Realworld-test
```

You can set the environment variable below to clean the database before running the tests.

```bash
export REALWORLD_TEST_TRUNCATE_DB=1
```

### Docker

You can build the Docker image on Docker-supported operating systems.

```bash
docker build -t realworld .
docker run -p 8000:8000 -e "REALWORLD_DB_HOST=<MySQL Server IP>" -e "REALWORLD_DB_PORT=<MySQL Server Port>" -e "REALWORLD_DB_NAME=<Database name>" -e "REALWORLD_DB_USER=<MySQL Server Username>" -e "REALWORLD_DB_PASSWORD=<MySQL Server Password>" -t realworld
```

### GitLab CI/CD

This project contains a GitLab CI/CD script. It automatically builds the executable and Docker image, runs the tests, and finally uploads the Docker image to docker.io. Refer to the [.gitlab-ci.yml](.gitlab-ci.yml) script for more details.

# 廣東話
# ![RealWorld 示範應用程式](img/logo.png)

![GitLab Pipeline](https://gitlab.com/yukaitung/realworld-oat-poco/badges/main/pipeline.svg)

[來自 GitHub？ 按此轉至 GitLab Repository](https://gitlab.com/yukaitung/realworld-oat-poco)

> ### C++、Oat++、POCO 現實應用原始碼 (CRUD、使用者驗證、MVCS pattern等) 符合 [RealWorld](https://github.com/gothinkster/realworld) 規格及 API。

本原始碼是用來展示以 **C++、Oat++、POCO** 建立的完整應用程式，包括 CRUD、使用者驗證、routing、分頁等功能。

如需更多關於如何與整合其他 Frontend / Backend 的資訊，請參考 [RealWorld 的 GitHub repository](https://github.com/gothinkster/realworld)。

### Links

*   [本項目的 GitLab Repository](https://gitlab.com/yukaitung/realworld-oat-poco)
*   [本項目的 GitHub Repository](https://github.com/yukaitung/realworld-oat-poco)
*   [本項目的 Docker Repository](https://hub.docker.com/r/yukaitung/realworld-oat-poco)

# 嘗試使用英式英文

所有 API endpoints, 參數及原始碼都使用英式英文。測試本項目時請將`favorite`更改為`favourite`。我為此特設英國版 [Postman Collection](test/Conduit.postman_collection_uk.json)。

# 軟件運作方式
### 軟件

本項目使用了 [Oat++](https://oatpp.io/) 以及 [POCO](https://pocoproject.org/)。以下是它們的功能

Oat++：

* 寄存 RESTful API (口語：Host 一個 RESTful API)
* 使用者驗證
* 校驗使用者輸入
* 執行 API 測試
* 整合 Swagger UI

POCO：

* 生產及驗證 JWT Token
* 編碼 / 解碼 [URI 編碼](https://docs.pocoproject.org/current/Poco.URI.html#20774)
* 連接 MySQL 數據庫
* 處理時差

### MVCS Pattern

本項目跟隨 MVCS pattern [(例子)](https://github.com/oatpp/example-crud) [(什麼是 MVCS)](https://stackoverflow.com/questions/5702391/mvcs-model-view-controller-service)。以下是本項目對 MVCS 的解釋。

* Controller 提供 endpoint 及選擇有關 Service
* Service 由 Model 提取及處理數據
* Model 在數據庫新增、更改、刪除數據

### 統一碼 (Unicode) 支援

大部份作業系統都支援統一碼(不太肯定視窗作業系統是否支援)。你可以用統一碼支援的語言新增資料。以下截圖表示了以廣東話在 macOS 新增資料。圖中 Slug 轉換為 [URI 編碼](https://docs.pocoproject.org/current/Poco.URI.html#20774)。

![Unicode Test](img/utf8.png)

# 如何使用

### 軟件要求

本項目需要在電腦上安裝 C++ 編譯器, CMake 2.23, [Conan 2.0](https://conan.io/), MySQL, 及 pip。在 macOS 可使用 [Homebrew](https://docs.brew.sh/Installation) 取代 pip。

在 Ubuntu 作業系統，你可以執行以下指令安裝所有軟件。CMake 最好透過 pip 安裝因為 Ubuntu 中 CMake 版本可能太舊。

```bash
sudo apt install -qy --no-install-recommends g++ make pip perl libmysqlclient-dev
sudo pip install conan cmake
sudo ln -s /usr/local/bin/cmake /usr/bin/cmake
```

在 macOS 作業系統首先要安裝 Xcode Command Line Tools。然後可以使用 Homebrew 安裝 conan。

```bash
brew install conan
```

然後執行以下指令[建立 conan profile](https://docs.conan.io/2/reference/commands/profile.html)

```bash
conan profile detect --force
```
### 編譯

要在 Linux / macOS 編譯項目，首先前往項目所在的資料夾內。執行以下指令，如果 MySQL 數據庫安裝在其他地方，請更改 `/usr/include/mysql`。

```bash
conan install . --output-folder=build --build=missing
cd build
cmake .. -DCMAKE_CXX_FLAGS=-isystem\ /usr/include/mysql -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . -j`nproc` # Linux
cmake --build . -j$(sysctl -n hw.ncpu) # macOS
```

### 執行程式

首先需要建立 SQL 數據庫。以下指令會建立一個名為 `Realworld` 的數據庫。這些指令忽略了網絡安全，只適合進行測試。數據庫設計可參考檔案 [realworld.sql](sql/realworld.sql)。

```bash
sudo mysql -e "CREATE USER 'admin'@'127.0.0.1' IDENTIFIED BY '123456';"
sudo mysql -e "GRANT ALL PRIVILEGES ON *.* TO 'admin'@'127.0.0.1';"
sudo mysql -e "CREATE DATABASE RealworldTest;"
sudo mysql RealworldTest < sql/realworld.sql
```

然後你需要設定 environment variables 方可執行程式。

| Variables Name    | Description |
| ----------------- | ----------- |
| REALWORLD_DB_HOST | MySQL 數據庫 IP 位址 |
| REALWORLD_DB_PORT | MySQL 數據庫 Port |
| REALWORLD_DB_NAME | 數據庫名稱 |
| REALWORLD_DB_USER | MySQL 數據庫使用者名稱 |
| REALWORLD_DB_PASSWORD | MySQL 數據庫密碼 |
| REALWORLD_SIGNER_SECRET | JWT Singer 密碼 |
| REALWORLD_CORS_ORIGIN | CORS Access-Control-Allow-Origin |

指令例字：

```bash
export REALWORLD_DB_HOST=127.0.0.1
export REALWORLD_DB_PORT=3306
export REALWORLD_DB_NAME=Realworld
export REALWORLD_DB_USER=admin
export REALWORLD_DB_PASSWORD=123456
export REALWORLD_SIGNER_SECRET=some_random_words
cd bin # The executable is in the bin folder
./Realworld
```

以下 Endpoints 可測試本項目

* http://localhost:8000/
* http://localhost:8000/swagger/ui

你可以使用 [Postman Collection](test/Conduit.postman_collection_uk.json) 來測試，或者在瀏覽器進入 [http://localhost:8000/swagger/ui](http://localhost:8000/swagger/ui) 使用 Swagger UI 來測試。

### 執行測試

如果你想執行測試，首先需要建立多一個 SQL 數據庫。然後設定 environment variables。

```bash
export REALWORLD_TEST_DB_HOST=127.0.0.1
export REALWORLD_TEST_DB_PORT=3306
export REALWORLD_TEST_DB_NAME=RealworldTest
export REALWORLD_TEST_DB_USER=admin
export REALWORLD_TEST_DB_PASSWORD=123456
export REALWORLD_TEST_SIGNER_SECRET=<some_random_words
./Realworld-test
```

你可以設定以下 environment variable ，當執行測試時會清除數據庫。

```bash
export REALWORLD_TEST_TRUNCATE_DB=1
```

### Docker

你可以在 Docker 支援的作業系統中建立一個 Docker image

```bash
docker build -t realworld .
docker run -p 8000:8000 -e "REALWORLD_DB_HOST=<MySQL Server IP>" -e "REALWORLD_DB_PORT=<MySQL Server Port>" -e "REALWORLD_DB_NAME=<Database name>" -e "REALWORLD_DB_USER=<MySQL Server Username>" -e "REALWORLD_DB_PASSWORD=<MySQL Server Password>" -t realworld
```

### GitLab CI/CD

這個 script 會自動編譯項目執行檔和 Docker image、執行測試、將 Docker image 上傳至 docker.io 。參考檔案 [.gitlab-ci.yml](.gitlab-ci.yml)。