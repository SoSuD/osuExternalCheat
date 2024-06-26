# ProjectAIP

## Описание

ProjectAIP — это приложение для автоматизации некоторых действий в игре osu!. Оно предоставляет функции для чтения данных из памяти процесса osu!, обработки и анализа данных карты, а также автоматического выполнения действий в игре.

## Функционал

- Чтение и обработка данных карты osu! из памяти процесса.
- Анализ и преобразование данных карты.
- Автоматическое выполнение действий в игре osu!, таких как перемещение курсора и нажатие клавиш.

## Установка

### Требования

- Windows 10 и выше
- Компилятор GCC (MinGW)
- CMake 3.10 и выше

### Шаги установки

1. Клонируйте репозиторий:

    ```bash
    git clone --recurse-submodules https://github.com/SoSuD/osuExternalCheat.git
    cd osuExternalCheat
    ```

2. Создайте и перейдите в директорию сборки:

    ```bash
    mkdir build
    cd build
    ```
3. Откройте файл main.cpp и отредактируйте 54 строчку под ваш путь к картам.
 
    ```std::string songsPath = {ваш путь};
        Пример: std::string songsPath = "C:/Users/SSD1337/AppData/Local/osu!/Songs/";
    ```

5. Сконфигурируйте проект с помощью CMake:

    ```bash
    cmake ..
    ```

6. Соберите проект:

    ```bash
    cmake --build .
    ```

## Использование

### Запуск приложения

1. Перейдите в директорию сборки:

    ```bash
    cd build
    ```

2. Запустите приложение:

    ```bash
    ./MainApp
    ```

## Структура проекта

    ```ProjectAIP/
    ├── build/ # Директория сборки
    ├── docs/ # Документация
    │ └── html/
    ├── func/ # Исходные файлы проекта
    │ ├── include/ # Заголовочные файлы
    │ └── src/ # Исходные файлы
    ├── CMakeLists.txt # Файл конфигурации CMake
    └── README.md # Этот файл
    ```


## Программные модули

### myfunctions.h и myfunctions.cpp

Эти файлы содержат функции для работы с процессами, чтения и записи памяти.

### geometry.h и geometry.cpp

Эти файлы содержат функции для вычисления и обработки геометрических данных, таких как координаты и перемещения.

### utils.h и utils.cpp

Эти файлы содержат вспомогательные функции для различных задач.

### config.h и config.cpp

Эти файлы содержат конфигурационные данные для работы с ними. (Разрешение экрана, множители пикселей и т.д.)

## Видео

[Демонстрация работы](https://youtu.be/5dQiheZRN3Q)

## Авторы

- Незметдинов Динар

## Лицензия

Этот проект лицензируется в соответствии с MIT License. Подробности см. в файле LICENSE.
