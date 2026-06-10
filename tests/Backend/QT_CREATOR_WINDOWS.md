# Как использовать Qt тесты в Qt Creator на Windows

## 🪟 Требования для Windows

### Необходимо установить:
- **Qt Creator** (последняя версия)
- **Qt 6.4+** или **Qt 5.15+** с компонентом **Qt Test**
- **CMake 3.21+**
- **MSVC 2019/2022** компилятор или **MinGW**

### Проверка установки Qt Test

Откройте Qt Creator:
1. **Tools** → **Options** → **Qt Versions**
2. Выберите вашу версию Qt
3. Убедитесь, что компонент **qtbase** включает **test**

## 📂 Открытие проекта в Qt Creator

### Способ 1: Через главный проект (Рекомендуется)

1. **Откройте** проект `PangaeaApps` в Qt Creator
   - File → Open File or Project
   - Выберите `CMakeLists.txt` из корня проекта

2. **Нажмите** Configure project

3. **Edit** → **Preferences** → **CMake**
   - Initial CMake arguments: `-DCMAKE_PREFIX_PATH=C:/Qt/6.4.0/msvc2022_64`
   - (Замените на ваш путь к Qt)

### Способ 2: Открыть только тесты

1. **File** → **Open File or Project**
2. Выберите `tests/Backend/CMakeLists.txt`
3. **Configure Project**

## ⚙️ Настройка для Windows

### Шаг 1: Установить пути Qt

**File** → **Preferences** → **Kits** → **Qt Versions**

```
Name:                    Qt 6.4 MSVC2022 64bit
qmake location:          C:/Qt/6.4.0/msvc2022_64/bin/qmake.exe
Autodetected:            No
Version:                 6.4.0
```

### Шаг 2: Создать Kit для тестов

**File** → **Preferences** → **Kits** → **Kits**

**Add Kit:**
```
Name:                    Test Kit
Device type:             Desktop
Compiler (C):            Microsoft Visual C++ Compiler 16.0 (amd64)
Compiler (C++):          Microsoft Visual C++ Compiler 16.0 (amd64)
Debugger:                cdb (debugging tools for Windows)
Qt version:              Qt 6.4 MSVC2022 64bit
CMake Tool:              CMake (system)
```

## 🏗️ Сборка тестов в Qt Creator

### Способ 1: Встроенная сборка

1. **Left Panel** → **Projects** tab
2. **Build & Run** → Select kit
3. **Edit** → **Build Steps**
4. Убедитесь что опция `-DBUILD_BACKEND_TESTS=ON` включена

```
Arguments:
-DCMAKE_PREFIX_PATH=C:/Qt/6.4.0/msvc2022_64 -DBUILD_BACKEND_TESTS=ON
```

### Способ 2: Сборка из Build Menu

1. **Build** → **Build All**
   - или используйте **Ctrl+B**

2. **Build** → **Build Project "PangaeaBackend"**

### Способ 3: Сборка конкретного теста

1. **Projects** → **Build Steps**
2. Нажмите на цель сборки (Target)
3. Выберите **TestDeviceParameter** или другой тест

## 🧪 Запуск тестов в Qt Creator

### Способ 1: Запустить все тесты

1. **Build** → **Run CMake** (если нужно)
2. **Build** → **Build All** (Ctrl+B)
3. **Tools** → **CMake** → **Run CTests**

Или из командной строки Qt Creator:
```
F5 - запустить проект
```

### Способ 2: Запустить конкретный тест

**Left Panel** → **Tests** tab:

1. Выберите категорию:
   - `TestDeviceParameter`
   - `TestParser`
   - `TestFirmware`
   - и т.д.

2. **Double-click** для запуска

3. **Right-click** → **Run Test**

### Способ 3: Запустить через Run Configurations

1. **Projects** → **Run**
2. **Executable**: `build/device/TestDeviceParameter.exe`
3. **Working directory**: `%{CurrentProject:Path}/build`
4. Нажмите **Run** (Ctrl+R)

## 📊 Просмотр результатов тестов

### Окно Test Results

Откроется автоматически после запуска теста:

```
[✓] Test Results
│
├─ TestDeviceParameter
│  ├─ ✓ testConstructor_EQ_VOLUME1     [PASS]
│  ├─ ✓ testSetValue                   [PASS]
│  ├─ ✓ testSetValueBoundaries         [PASS]
│  └─ ✗ testParameterType              [FAIL]
│
├─ TestParser
│  ├─ ✓ testParserCreation             [PASS]
│  └─ ✓ testParserAddCommandHandler    [PASS]
│
└─ Total: 47 tests, 45 passed, 2 failed
```

### Детали теста

**Click** на тест → **Output** tab:

```
FAIL! : TestDeviceParameter::testParameterType()
Loc: [file.cpp(line)]
    'param->parameterType()' returned value doesn't match
    Expected: 87
    Actual:   42

Totals: 0 passed, 1 failed, 0 skipped, 0 blacklisted, 0ms
```

## 🐛 Отладка тестов в Qt Creator

### Метод 1: Отладка конкретного теста

1. **Double-click** на название теста в левой панели
2. **Right-click** → **Debug Test**
   - или нажмите **Shift+F5**

### Метод 2: Установить точку останова

1. **Откройте** файл теста: `tests/Backend/device/tst_deviceparameter.cpp`

2. **Left-click** на номер строки чтобы поставить точку:
   ```cpp
   void TestDeviceParameter::testSetValue()
   {
       QVERIFY(param != nullptr);  // ← Click здесь
       param->setValue(50);
       QCOMPARE(param->value(), 50);
   }
   ```

3. **Debug** → **Start Debugging** (F5)

4. После нажатия точки останова используйте:
   - **F10** - Step over
   - **F11** - Step into
   - **Shift+F11** - Step out
   - **F5** - Continue

### Метод 3: Просмотр переменных

Во время отладки:
1. **Left Panel** → **Locals** tab
2. Видите все переменные
3. **Expand** объекты для просмотра свойств

## 📋 Запуск через терминал Qt Creator

### Встроенный терминал

1. **Tools** → **Terminal** (если нет, то **Tools** → **Options** → **Environment**)

2. Выполните команды:

```bash
# Перейти в директорию build
cd d:\AMT\Applications\PangaeaApps\build

# Запустить все тесты
ctest

# Запустить с деталями
ctest --verbose

# Запустить конкретный тест
ctest -R TestDeviceParameter

# Запустить конкретный exe
.\device\TestDeviceParameter.exe

# Запустить с выводом ошибок
ctest --output-on-failure
```

## 📈 Измерение покрытия кода в Qt Creator

### Шаг 1: Установить инструменты покрытия

Для Windows с MSVC:

1. **Tools** → **Options** → **Analyzer**
2. Установить инструмент покрытия (если доступно)

### Шаг 2: Включить покрытие при компиляции

**Edit** → **Build & Run** → **CMake**:

```
Arguments: -DCMAKE_CXX_FLAGS="/Oy-" -DBUILD_BACKEND_TESTS=ON
```

### Шаг 3: Запустить анализ

```
Analyze → Code Coverage...
```

Результаты покажут процент покрытия по файлам.

## 🎯 Полный рабочий цикл в Qt Creator

### 1️⃣ Инициальная настройка (один раз)

```
File → Open File or Project
  → Select: CMakeLists.txt (из корня)
  → Configure project
  
File → Preferences → Kits
  → Создать kit для тестов
  
File → Preferences → CMake
  → Установить CMAKE_PREFIX_PATH
```

### 2️⃣ Разработка и тестирование

```
1. Редактируем код в PangaeaBackend/
2. Ctrl+B - Собрать проект
3. Tools → CMake → Run CTests (запустить все тесты)
4. Смотрим результаты в Test Results
5. Если нужна отладка: Shift+F5 на конкретном тесте
```

### 3️⃣ Добавление нового теста

```
1. Создаем файл: tests/Backend/category/tst_mytest.cpp
2. Редактируем CMakeLists.txt в этой папке
3. Ctrl+B - Собрить новый тест
4. Double-click на новом тесте в Tests tab
5. Shift+F5 - Отладить если нужно
```

## 🔧 Решение проблем на Windows

### Проблема: "Qt не найден"

**Решение:**
1. **File** → **Preferences** → **CMake**
2. Добавьте: `-DCMAKE_PREFIX_PATH=C:/Qt/6.4.0/msvc2022_64`
3. Нажмите **Apply**
4. **Build** → **Run CMake**

### Проблема: "MSVC компилятор не найден"

**Решение:**
1. Установите Visual Studio Build Tools или Visual Studio
2. **File** → **Preferences** → **Kits**
3. Нажмите **Auto-detect** для поиска компилятора

### Проблема: "Тесты не отображаются в Tests tab"

**Решение:**
1. **Build** → **Clean All**
2. **Build** → **Build All**
3. **Tools** → **CMake** → **Run CTests** (обновляет список)

### Проблема: "ctest: command not found" в терминале

**Решение:**
```
# В Qt Creator терминале используйте:
cd d:\AMT\Applications\PangaeaApps\build
ctest

# Или полный путь:
C:\CMake\bin\ctest.exe --verbose
```

### Проблема: Тесты компилируются но не запускаются

**Решение:**
1. **Projects** → **Run Settings**
2. **Executable**: `%{BuildDirectory}/device/TestDeviceParameter.exe`
3. **Working Directory**: `%{BuildDirectory}`

## 💡 Полезные сочетания клавиш

| Клавиша | Действие |
|---------|----------|
| **Ctrl+B** | Собрать проект |
| **Ctrl+R** | Запустить проект |
| **F5** | Запустить с отладкой |
| **Shift+F5** | Отладить текущий тест |
| **F10** | Step over (отладка) |
| **F11** | Step into (отладка) |
| **Ctrl+Shift+B** | Пересобрать все |
| **Ctrl+K** | Откомпилировать файл |

## 📝 Пример: Запуск одного теста на Windows

### Пошагово:

1. **Откройте** Qt Creator

2. **File** → **Open File or Project**
   - Путь: `d:\AMT\Applications\PangaeaApps\CMakeLists.txt`
   - **Next** → **Finish**

3. **Дождитесь** Configure Project

4. **Projects** → **Left Panel** → **Tests** tab

5. **Expand** → Device → TestDeviceParameter

6. **Right-click** на `testSetValue`

7. **Run Selected Tests**

8. **Результат** в Test Results:
   ```
   ✓ testSetValue() PASS (125ms)
   ```

9. **Если нужна отладка**: **Debug Selected Tests** вместо Run

## 🎓 Расширенные возможности

### Просмотр логов CTest

**Tools** → **General** → **Show Project-Level Edit Menu**:

```bash
ctest --verbose --output-on-failure > test_log.txt 2>&1
```

### Фильтрация тестов

В терминале Qt Creator:

```bash
# Запустить только device тесты
cd build
ctest -R "TestDevice.*"

# Запустить кроме FileBrowser
ctest -E "FileBrowser"

# Только Parser тесты
ctest -R Parser --verbose
```

### Параллельный запуск

```bash
# Запустить 4 теста одновременно
ctest -j 4

# Или все процессоры
ctest -j
```

## 📚 Дополнительные ресурсы

- **Документация тестов**: `tests/Backend/README.md`
- **Гайд по Qt Test**: `tests/Backend/QT_TEST_GUIDE.md`
- **Интеграция**: `tests/Backend/INTEGRATION_GUIDE.md`
- **Быстрая справка**: `tests/Backend/QUICK_REFERENCE.md`

---

## ✅ Чеклист для первого запуска

- [ ] Qt Creator установлен
- [ ] Qt 6.4+ установлен (с Test компонентом)
- [ ] CMake 3.21+ установлен
- [ ] MSVC компилятор установлен
- [ ] Проект открыт в Qt Creator
- [ ] CMAKE_PREFIX_PATH установлен
- [ ] Проект сконфигурирован (Configure Project)
- [ ] Проект собран (Ctrl+B)
- [ ] Видны тесты в Tests tab
- [ ] Первый тест запущен успешно

**Готово! Теперь вы можете использовать Qt тесты в Qt Creator на Windows** 🎉
