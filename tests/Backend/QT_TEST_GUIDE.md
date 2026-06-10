# Qt Test Framework Quick Start Guide for PangaeaBackend Tests

## Overview

This guide provides practical examples of using Qt's Test framework (QtTest) for the PangaeaBackend unit tests. It covers common patterns, best practices, and examples specific to this project.

## Table of Contents

1. [Basic Test Structure](#basic-test-structure)
2. [Writing Effective Tests](#writing-effective-tests)
3. [Assertions Reference](#assertions-reference)
4. [Testing Signals and Slots](#testing-signals-and-slots)
5. [Data-Driven Tests](#data-driven-tests)
6. [Common Patterns in PangaeaBackend Tests](#common-patterns-in-pangaeabackend-tests)
7. [Debugging Tests](#debugging-tests)

## Basic Test Structure

### Minimal Test Example

```cpp
#include <QtTest>

class TestMyClass : public QObject
{
    Q_OBJECT

private slots:
    // These are automatically discovered and run by QTest
    void testFeature1();
    void testFeature2();
};

void TestMyClass::testFeature1()
{
    // Arrange
    MyClass obj;
    
    // Act
    int result = obj.calculate(5);
    
    // Assert
    QCOMPARE(result, 10);
}

void TestMyClass::testFeature2()
{
    QVERIFY(true); // Should pass
}

QTEST_MAIN(TestMyClass)
#include "tst_myclass.moc"
```

### Test Lifecycle Methods

```cpp
class TestExample : public QObject
{
    Q_OBJECT

private slots:
    // Called once before any test
    void initTestCase();
    
    // Called before each test
    void init();
    
    // The actual tests
    void testOne();
    void testTwo();
    
    // Called after each test
    void cleanup();
    
    // Called once after all tests
    void cleanupTestCase();
};

void TestExample::initTestCase()
{
    qInfo() << "Test suite starting...";
}

void TestExample::init()
{
    qInfo() << "Individual test starting...";
}

void TestExample::testOne()
{
    QVERIFY(true);
}

void TestExample::cleanup()
{
    qInfo() << "Individual test cleanup...";
}

void TestExample::cleanupTestCase()
{
    qInfo() << "Test suite cleanup...";
}
```

## Writing Effective Tests

### Good Test Practices

#### ✅ DO: Test One Thing Per Test

```cpp
// GOOD: Each test has a single focus
void TestDeviceParameter::testSetValue()
{
    DeviceParameter param(DeviceParameter::Type::MASTER_VOLUME);
    param.setValue(50);
    QCOMPARE(param.value(), 50);
}

void TestDeviceParameter::testSetValueBoundary()
{
    DeviceParameter param(DeviceParameter::Type::MASTER_VOLUME);
    param.setValue(255);
    QCOMPARE(param.value(), 255);
}
```

#### ❌ DON'T: Test Multiple Things

```cpp
// BAD: Multiple assertions test different behaviors
void TestDeviceParameter::testParameter()
{
    DeviceParameter param(DeviceParameter::Type::MASTER_VOLUME);
    param.setValue(50);
    QCOMPARE(param.value(), 50);      // Test 1: Setting
    QVERIFY(param.isValid());           // Test 2: Validation
    QString str = param.toString();     // Test 3: String conversion
    QVERIFY(!str.isEmpty());
}
```

#### ✅ DO: Use Descriptive Names

```cpp
// GOOD: Clear what is being tested
void TestParser::testParseCommandWithValidData();
void TestParser::testParseCommandWithEmptyBuffer();
void TestParser::testParseCommandWithMissingTerminator();
```

#### ❌ DON'T: Use Generic Names

```cpp
// BAD: Not clear what's being tested
void TestParser::testParse();
void TestParser::test1();
void TestParser::testCommand();
```

#### ✅ DO: Test Both Success and Failure Cases

```cpp
void TestParser::testParseLegitimateCommand()
{
    Parser parser;
    QList<QByteArray> result = parser.parseNewData("VALID_COMMAND\n");
    QVERIFY(!result.isEmpty());
}

void TestParser::testParseInvalidCommand()
{
    Parser parser;
    QList<QByteArray> result = parser.parseNewData("INVALID");
    // Verify appropriate behavior for invalid input
}
```

#### ✅ DO: Clean Up Resources

```cpp
void TestDeviceParameter::cleanupTestCase()
{
    if (param) {
        delete param;
        param = nullptr;
    }
}
```

## Assertions Reference

### Verification Assertions

```cpp
// Basic verification
QVERIFY(condition);                    // Pass if true
QVERIFY2(condition, "message");        // With message

// Compare values
QCOMPARE(actual, expected);            // Deep comparison
QTEST_ASSERT(condition);               // Assert and abort

// Floating point comparison
QCOMPARE(actual, expected);            // For qreal, accounts for precision

// String testing
QVERIFY(string.contains("substring"));
QVERIFY(string.startsWith("prefix"));
QCOMPARE(string, "expected");
```

### Container Assertions

```cpp
// Lists and arrays
QVERIFY(list.isEmpty());
QCOMPARE(list.size(), expectedSize);
QVERIFY(list.contains(item));

// QByteArray
QVERIFY(data.contains("pattern"));
QCOMPARE(data.size(), 10);

// Maps
QVERIFY(map.contains(key));
QCOMPARE(map.value(key), expected);
```

### Signal Testing

```cpp
// Create signal spy
QSignalSpy spy(object, &ObjectClass::signalEmitted);

// Trigger action
doSomething();

// Verify signal was emitted
QTRY_VERIFY(spy.count() > 0);
QCOMPARE(spy.count(), 1);

// Check signal arguments
QCOMPARE(spy.at(0).at(0), expectedFirstArg);
```

### Failure Assertions

```cpp
// Expected to fail
QEXPECT_FAIL("context", "reason", Continue);
QVERIFY(false); // Will not stop test

// Conditional fail
if (condition) {
    QFAIL("Something went wrong");
}
```

## Testing Signals and Slots

### Example: Testing DeviceParameter Signal

```cpp
class TestDeviceParameterSignals : public QObject
{
    Q_OBJECT

private slots:
    void testValueChangedSignal()
    {
        DeviceParameter param(DeviceParameter::Type::MASTER_VOLUME);
        
        // Create spy to monitor signal
        QSignalSpy spy(&param, &DeviceParameter::valueChanged);
        
        // Trigger action that should emit signal
        param.setValue(50);
        
        // Verify signal was emitted
        QTRY_VERIFY(spy.count() > 0);
        
        // Verify signal parameter
        QCOMPARE(spy.at(0).at(0).toInt(), 50);
    }
};
```

### Example: Testing Connections

```cpp
class TestCore : public QObject
{
    Q_OBJECT

private slots:
    void testCoreEmitsDeviceChanged()
    {
        Core core;
        AbstractDevice* device = new MockDevice();
        
        // Create spy
        QSignalSpy spy(&core, &Core::sgCurrentDeviceChanged);
        
        // Simulate device connection
        core.slInterfaceConnected(DeviceDescription{"MockDevice", "virtual", Offline});
        
        // Wait for signal with timeout
        QTRY_VERIFY_WITH_TIMEOUT(spy.count() > 0, 1000);
    }
};
```

## Data-Driven Tests

### Using Test Data Table

```cpp
class TestDeviceParameter : public QObject
{
    Q_OBJECT

private slots:
    void testMultipleValues_data();
    void testMultipleValues();
};

void TestDeviceParameter::testMultipleValues_data()
{
    QTest::addColumn<DeviceParameter::Type>("paramType");
    QTest::addColumn<quint8>("setValue");
    QTest::addColumn<quint8>("expectedValue");
    
    QTest::newRow("volume_0") << DeviceParameter::Type::MASTER_VOLUME << 0 << 0;
    QTest::newRow("volume_50") << DeviceParameter::Type::MASTER_VOLUME << 50 << 50;
    QTest::newRow("volume_255") << DeviceParameter::Type::MASTER_VOLUME << 255 << 255;
}

void TestDeviceParameter::testMultipleValues()
{
    QFETCH(DeviceParameter::Type, paramType);
    QFETCH(quint8, setValue);
    QFETCH(quint8, expectedValue);
    
    DeviceParameter param(paramType);
    param.setValue(setValue);
    QCOMPARE(param.value(), expectedValue);
}
```

## Common Patterns in PangaeaBackend Tests

### Pattern 1: Testing Device Parameters

```cpp
void TestDeviceParameter::testParameterSetAndGet()
{
    // Setup
    DeviceParameter param(DeviceParameter::Type::MASTER_VOLUME);
    quint8 testValue = 75;
    
    // Execute
    param.setValue(testValue);
    
    // Verify
    QCOMPARE(param.value(), testValue);
    QCOMPARE(param.parameterType(), DeviceParameter::Type::MASTER_VOLUME);
}
```

### Pattern 2: Testing Model Behavior

```cpp
void TestPresetListModel::testModelRowCount()
{
    PresetListModel model;
    
    int initialCount = model.rowCount();
    QVERIFY(initialCount >= 0);
    
    // If model supports adding items, test that too
    // model.addPreset(newPreset);
    // QCOMPARE(model.rowCount(), initialCount + 1);
}
```

### Pattern 3: Testing with Temporary Files

```cpp
#include <QTemporaryDir>

void TestIRFile::testSaveAndLoad()
{
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    IRFile original;
    original.setName("TestIR");
    original.setData("IR response data");
    
    // Save to temp file
    QString filePath = tempDir.path() + "/test.ir";
    QVERIFY(original.saveToFile(filePath));
    
    // Load from temp file
    IRFile loaded;
    QVERIFY(loaded.loadFromFile(filePath));
    QCOMPARE(loaded.name(), original.name());
}
```

### Pattern 4: Testing with Thread Safety

```cpp
class TestThreadedCore : public QObject
{
    Q_OBJECT

private slots:
    void testCoreThreadSafety()
    {
        Core core;
        core.moveToThread(new QThread());
        
        QSignalSpy spy(&core, &Core::sgCurrentDeviceChanged);
        
        // Emit signal to interface core in different thread
        QMetaObject::invokeMethod(&core, "slInterfaceConnected", Qt::QueuedConnection);
        
        // Wait for signal across threads
        QTRY_VERIFY_WITH_TIMEOUT(spy.count() > 0, 1000);
    }
};
```

## Debugging Tests

### Enable Verbose Output

```bash
# Run with verbose output to see all test names
./TestParser -v2

# Or with environment variable
QT_QPA_QT_DEBUG_BACKINGSTORE=1 ./TestParser
```

### Debug in Qt Creator

1. Open test file
2. Set breakpoint in test method
3. Right-click test name → Debug Selected Tests
4. Debugger will stop at breakpoints

### Print Debug Information

```cpp
void TestParser::testParseCommand()
{
    Parser parser;
    QList<QByteArray> result = parser.parseNewData("TEST\n");
    
    qDebug() << "Parser result count:" << result.size();
    foreach (QByteArray item, result) {
        qDebug() << "Result item:" << item;
    }
    
    QVERIFY(result.size() > 0);
}
```

### Common Debugging Issues

#### Test Won't Compile

```cpp
// Ensure CMakeLists.txt includes all required files:
// - Source files (.cpp)
// - Header files (.h)
// - Qt linking (Qt::Test, Qt::Core, etc.)
```

#### Test Crashes

```cpp
// Check for:
// 1. Null pointer dereferences
// 2. Objects not initialized in initTestCase()
// 3. Resources not cleaned up in cleanupTestCase()

void TestMyClass::initTestCase()
{
    obj = new MyClass(); // Allocate
}

void TestMyClass::cleanupTestCase()
{
    delete obj;          // Deallocate
    obj = nullptr;
}
```

#### Signal Tests Fail

```cpp
// Use QTRY_VERIFY for asynchronous signals
QSignalSpy spy(object, &Object::signal);
doSomething();

// WRONG: Signal may not be emitted yet
QVERIFY(spy.count() > 0);

// CORRECT: Wait up to 5000ms for signal
QTRY_VERIFY_WITH_TIMEOUT(spy.count() > 0, 5000);
```

## Running Tests with CTest

### Basic Commands

```bash
# Run all tests
ctest

# Run tests with output
ctest --output-on-failure

# Run specific test
ctest -R TestParser

# Run tests matching pattern
ctest -R "Test.*Device.*"

# Run with verbose output
ctest --verbose

# Run tests in parallel
ctest -j 4

# Stop on first failure
ctest --stop-on-failure
```

### Continuous Integration Example

```bash
#!/bin/bash
set -e

echo "Building tests..."
cd build
cmake --build . --target all

echo "Running tests..."
ctest --verbose --output-on-failure

echo "Tests completed successfully!"
```

## Resources

- [Qt Test Documentation](https://doc.qt.io/qt-6/qttest-index.html)
- [QTest Namespace](https://doc.qt.io/qt-6/qtest.html)
- [QSignalSpy](https://doc.qt.io/qt-6/qsignalspy.html)
- [Qt Creator Testing](https://doc.qt.io/qtcreator/creator-testing.html)

## Summary

Key takeaways for effective Qt testing:

1. **One test per feature** - Keep tests focused and specific
2. **Use clear names** - Make test purposes obvious
3. **Verify both success and failure** - Test edge cases
4. **Clean up resources** - Avoid memory leaks
5. **Wait for signals** - Use QTRY_VERIFY for async operations
6. **Run regularly** - Include tests in CI/CD pipeline
7. **Measure coverage** - Aim for >80% code coverage

Happy testing!
