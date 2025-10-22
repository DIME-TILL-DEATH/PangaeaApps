#include "qserialport.h"

#include <QCoreApplication>
#include <qnativeinterface.h>

using namespace QtJniTypes;

static char UsbSerial_jniClassName[] {"org/jniusbserial/JniUsbSerial"};

static void jniDeviceNewData(JNIEnv *envA, jobject thizA, jlong classPoint, jbyteArray dataA)
{
    Q_UNUSED(thizA);

    if (classPoint != 0)
    {
        jbyte *bytesL = envA->GetByteArrayElements(dataA, NULL);
        jsize lenL = envA->GetArrayLength(dataA);
        ((QSerialPort *)classPoint)->newDataArrived((char *)bytesL, lenL);
        envA->ReleaseByteArrayElements(dataA, bytesL, JNI_ABORT);
    }
}

static void jniDeviceException(JNIEnv *envA, jobject thizA, jlong classPoint, jstring messageA)
{
    Q_UNUSED(thizA);

    if (classPoint != 0)
    {
        const char *stringL = envA->GetStringUTFChars(messageA, NULL);
        QString strL = QString::fromUtf8(stringL);
        envA->ReleaseStringUTFChars(messageA, stringL);
        if (envA->ExceptionCheck())
            envA->ExceptionClear();
        ((QSerialPort *)classPoint)->exceptionArrived(strL);
    }
}

QSerialPort::QSerialPort(QObject *parent)
    :QObject(parent)
{
    readBufferMaxSize = 16384;

    isConnected = false;
    m_portName = "";
    m_baudRate = Baud115200;
    m_dataBits = Data8;
    m_parity = NoParity;
    m_stopBits = OneStop;

    JNINativeMethod methodsL[] {{"nativeDeviceNewData", "(J[B)V", reinterpret_cast<void *>(jniDeviceNewData)},
                                        {"nativeDeviceException", "(ILjava/lang/String;)V", reinterpret_cast<void *>(jniDeviceException)}};

    QJniEnvironment envL;
    QJniObject javaClassL(UsbSerial_jniClassName);
    jclass objectClassL = envL->GetObjectClass(javaClassL.object<jobject>());

    envL->RegisterNatives(objectClassL, methodsL, sizeof(methodsL) / sizeof(methodsL[0]));
    envL->DeleteLocalRef(objectClassL);

    if (envL->ExceptionCheck())
        envL->ExceptionClear();


    QJniObject::callStaticMethod<void>(UsbSerial_jniClassName,
                                       "setActivity",
                                       "(Landroid/app/Activity;)V",
                                       QNativeInterface::QAndroidApplication::context().object());
}


bool QSerialPort::setBaudRate(qint32 baudRate)
{
    m_baudRate = baudRate;

    return setParameters();
}

void QSerialPort::newDataArrived(char *bytesA, int lengthA)
{
    int bytesToReadL = lengthA;

    if (readBufferMaxSize && (bytesToReadL > (readBufferMaxSize - readBuffer.size()))) {
        bytesToReadL = readBufferMaxSize - readBuffer.size();
        if (bytesToReadL <= 0) {
            stopReadThread();
            return;
        }
    }

    readBuffer.open(QIODevice::ReadWrite);
    readBuffer.seek(readBuffer.size());
    readBuffer.write(bytesA, bytesToReadL);
    readBuffer.close();

    emit readyRead();
}

void QSerialPort::exceptionArrived(QString strA)
{
    Q_UNUSED(strA);
    emit errorOccurred(ResourceError);
}

void QSerialPort::stopReadThread()
{
    QJniObject java_portName = QJniObject::fromString(m_portName);
    QJniObject::callStaticMethod<void>(UsbSerial_jniClassName,
                                              "stopIoManager",
                                              "(Ljava/lang/String;)V",
                                              java_portName.object<jstring>());
}

void QSerialPort::startReadThread()
{
    QJniObject java_portName = QJniObject::fromString(m_portName);
    QJniObject::callStaticMethod<void>(UsbSerial_jniClassName,
                                              "startIoManager",
                                              "(Ljava/lang/String;J)V",
                                              java_portName.object<jstring>(),
                                              (jlong)this);
}

qint64 QSerialPort::bytesAvailable()
{
    return readBuffer.size();
}

QByteArray QSerialPort::read(qint64 maxlen)
{
    if (maxlen > readBuffer.size())
    {
        return readAll();
    }

    readBuffer.open(QIODevice::ReadWrite);
    const QByteArray &data = readBuffer.data();
    const QByteArray firstNBytes = data.left(maxlen);
    readBuffer.buffer().remove(0, maxlen);
    readBuffer.close();

    return firstNBytes;
}

QByteArray QSerialPort::readAll()
{
    readBuffer.open(QIODevice::ReadWrite);
    const QByteArray data = readBuffer.data();
    readBuffer.buffer().clear();
    readBuffer.close();

    return data;
}

QByteArray QSerialPort::readLine()
{
    readBuffer.open(QIODevice::ReadWrite);
    const QByteArray data = readBuffer.readLine();
    readBuffer.buffer().remove(0, data.length());
    readBuffer.close();

    return data;
}

bool QSerialPort::deviceHasCapable()
{
    return QJniObject::callStaticMethod<jboolean>(UsbSerial_jniClassName,
                                       "hasCapable",
                                       "(Landroid/app/Activity;)Z",
                                       QNativeInterface::QAndroidApplication::context().object());
}

bool QSerialPort::isOpen()
{
    return isConnected;
}

void QSerialPort::close()
{
    if (m_portName == "")
    {
        isConnected = false;
        return;
    }

    QJniObject java_portName = QJniObject::fromString(m_portName);
    jboolean resultL = QJniObject::callStaticMethod<jboolean>(UsbSerial_jniClassName,
                                                                         "close",
                                                                         "(Ljava/lang/String;)Z",
                                                                         java_portName.object<jstring>());

    if (resultL)
    {
        isConnected = false;
    }
}

bool QSerialPort::open(QIODeviceBase::OpenMode mode)
{
    Q_UNUSED(mode);

    if (m_portName == "")
    {
        return false;
    }

    QJniObject java_portName = QJniObject::fromString(m_portName);
    jint resultL = QJniObject::callStaticMethod<jint>(UsbSerial_jniClassName,
                                                                         "open",
                                                                         "(Ljava/lang/String;J)I",
                                                                         java_portName.object<jstring>(),
                                                                         (jlong)this);

    // qDebug() << "jint result:" << resultL;
    // int resultL = javaSerialPort.callMethod<int>("open", m_portName, (jlong)this);

    if(resultL == 127)
    {
        isConnected = true;
        if (!setParameters())
        {
            isConnected = false;
            return false;
        }
        return true;
    }

    return false;
}

qint64 QSerialPort::write(const QByteArray &data)
{
    return write(data.constData(), data.size());
}

qint64 QSerialPort::write(const char *data, qint64 maxSize)
{
    if (m_portName == "")
    {
        emit errorOccurred(QSerialPort::NotOpenError);
        return 0;
    }

    if (!isConnected)
    {
        emit errorOccurred(QSerialPort::NotOpenError);
        return 0;
    }

    QJniEnvironment envL;
    QJniObject java_portName = QJniObject::fromString(m_portName);
    jbyteArray jarrayL = envL->NewByteArray(maxSize);
    envL->SetByteArrayRegion(jarrayL, 0, maxSize, (jbyte *)data);
    int resultL = QJniObject::callStaticMethod<jint>(UsbSerial_jniClassName,
                                                            "write",
                                                            "(Ljava/lang/String;[BI)I",
                                                            java_portName.object<jstring>(),
                                                            jarrayL,
                                                            2000);

    if (envL->ExceptionCheck())
    {
        envL->ExceptionClear();
        emit errorOccurred(QSerialPort::UnknownError);
        envL->DeleteLocalRef(jarrayL);
        return 0;
    }

    envL->DeleteLocalRef(jarrayL);

    if (resultL == 0)
    {
        emit errorOccurred(QSerialPort::WriteError);
        return 0;
    }

    return maxSize;
}

void QSerialPort::setPortName(const QString &name)
{
    m_portName = name;
}

QString QSerialPort::portName() const
{
    return m_portName;
}

bool QSerialPort::setParameters()
{
    if (isConnected == true)
    {
        QJniObject java_portName = QJniObject::fromString(m_portName);

        jboolean resultL = QJniObject::callStaticMethod<jboolean>(UsbSerial_jniClassName,
                                           "setParameters",
                                           "(Ljava/lang/String;IIII)Z",
                                            java_portName.object<jstring>(),
                                               m_baudRate,
                                               m_dataBits,
                                               m_stopBits,
                                               m_parity);

        return resultL;
    }

    return true;
}


