if(ANDROID)
    target_include_directories(${PROJECT_NAME} PUBLIC
        android/src/
    )

    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt::CorePrivate)

    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_VERSION_CODE ${VERSION_CODE}
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
    )
    set_property(TARGET Pangaea-mobile PROPERTY QT_ANDROID_EXTRA_LIBS
       ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto_1_1.so
       ${CMAKE_CURRENT_SOURCE_DIR}/android_ssl_libs/${CMAKE_ANDROID_ARCH_ABI}/libssl_1_1.so
       ${CMAKE_CURRENT_SOURCE_DIR}/android_openmp_libs/${CMAKE_ANDROID_ARCH_ABI}/libomp.so #needed for android sox lib

       ${CMAKE_CURRENT_SOURCE_DIR}/../../sox_lib/lib.android/${CMAKE_ANDROID_ARCH_ABI}/libsox.so
    )

    qt6_add_resources(Pangaea-mobile "AndroidResources"
        PREFIX
            "/"
        FILES
            android/src/JavaFile.java

            android/AndroidManifest.xml
            android/build.gradle
    )
endif()

add_subdirectory(qml)
add_subdirectory(qml/ControlGroups/)
add_subdirectory(qml/CustomOverlays/)
add_subdirectory(qml/Elements/)
add_subdirectory(qml/Modules/)
add_subdirectory(qml/Pages/)
add_subdirectory(qml/StyleSettings/)
add_subdirectory(qml/Tutorials/)
