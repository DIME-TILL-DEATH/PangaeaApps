# R8 / ProGuard rules for Qt Android release builds.
# Keep the exact Java entry points used by native/Qt JNI calls.

-keep class org.qtproject.qt.android.bindings.** { *; }
-keep class org.qtproject.qt5.android.bindings.** { *; }
-keep class org.qtproject.qt6.android.bindings.** { *; }

-keep class com.amtelectronics.MainActivity {
    public <init>();
    public void onCreate(android.os.Bundle);
}

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

# General Android release optimization recommendations.
-optimizations !class/merging/*,!code/simplification/arithmetic,!code/simplification/cast,!field/*,!method/*
-optimizationpasses 5
-dontwarn org.qtproject.qt.android.bindings.**
-dontwarn org.qtproject.qt5.android.bindings.**
-dontwarn org.qtproject.qt6.android.bindings.**

# Keep JNI bridge entry points visible to the runtime.
-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod
-keepattributes RuntimeVisibleAnnotations
