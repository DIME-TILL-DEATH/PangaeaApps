# R8 / ProGuard rules for Qt Android release builds.
# Keep the exact Java entry points used by native/Qt JNI calls and the launcher activity.

-keep class org.qtproject.qt.android.** { *; }
-keep class org.qtproject.qt5.android.** { *; }
-keep class org.qtproject.qt6.android.** { *; }
-keep class qt.android.** { *; }
-keep class qml.** { *; }
-keep class qt.qml.** { *; }

# Launcher activity must stay present for Android manifest resolution.
-keep class com.amtelectronics.MainActivity { *; }
-keep class com.amtelectronics.** { *; }

# JNI bridge entry points used from Qt/C++ should never be removed.
-keep class com.amtelectronics.utils.JavaFile {
    public static void takeReadUriPermission(android.net.Uri, android.content.Context);
    public static void takeWriteUriPermission(android.net.Uri, android.content.Context);
    public static java.lang.String getFileName(android.net.Uri, android.content.Context);
    public static void openFileExternally(java.lang.String, android.content.Context);
}

# Keep Android classes referenced by the manifest / file provider / activity startup.
-keep class androidx.core.content.FileProvider { *; }
-keep class android.app.Activity { *; }
-keep class android.content.Intent { *; }

# Native
-keepclasseswithmembernames class * {
    native <methods>;
}

# Сохраняем классы, которые упоминаются в AndroidManifest (Activity, Service и т.д.)
-keep public class * extends android.app.Activity
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider

# General Android release optimization recommendations.
-optimizations !class/merging/*,!code/simplification/arithmetic,!code/simplification/cast,!field/*,!method/*
-optimizationpasses 5
-dontwarn org.qtproject.qt.android.bindings.**
-dontwarn org.qtproject.qt5.android.bindings.**
-dontwarn org.qtproject.qt6.android.bindings.**

# Keep JNI bridge entry points visible to the runtime.
-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod
-keepattributes RuntimeVisibleAnnotations
