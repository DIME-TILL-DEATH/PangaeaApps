if(ANDROID)
    target_include_directories(${PROJECT_NAME} PUBLIC
        android/src/
    )

    add_android_openssl_libraries(${PROJECT_NAME})

    target_link_libraries(${PROJECT_NAME} PUBLIC omp)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE ${VERSION_CODE}
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
    set_property(TARGET ${PROJECT_NAME} PROPERTY QT_ANDROID_EXTRA_LIBS
       ${CMAKE_CURRENT_SOURCE_DIR}/android_openmp_libs/${CMAKE_ANDROID_ARCH_ABI}/libomp.so #needed for android sox lib
       ${CMAKE_CURRENT_SOURCE_DIR}/../../sox_lib/lib.android/${CMAKE_ANDROID_ARCH_ABI}/libsox.so
    )

    qt6_add_resources(${PROJECT_NAME} "AndroidResources"
        PREFIX
            "/"
        FILES
            android/src/JavaFile.java
            android/src/MainActivity.java
            android/src/org/jniusbserial/JniUsbSerial.java
            android/src/org/jniusbserial/SerialInputOutputManager.java

            android/AndroidManifest.xml
            android/res/values/splashscreentheme.xml
            android/res/values/libs.xml

            android/build.gradle
            android/gradle.properties
            android/proguard-rules.pro
    )
endif()

add_subdirectory(qml/Elements/)
add_subdirectory(qml/StyleSettings/)
add_subdirectory(qml/ControlGroups/)
add_subdirectory(qml/CustomOverlays/)
add_subdirectory(qml/Layouts/)
add_subdirectory(qml/ModulesClassic/)
add_subdirectory(qml/ModulesFX/)
add_subdirectory(qml/CP16/)
add_subdirectory(qml/CP100FX/)

add_subdirectory(qml/Tutorials/)

