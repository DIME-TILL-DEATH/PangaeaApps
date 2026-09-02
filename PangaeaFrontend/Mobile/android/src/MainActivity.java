package com.amtelectronics;

import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import androidx.core.view.WindowCompat;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Window window = getWindow();
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        window.setBackgroundDrawable(new ColorDrawable(Color.BLACK));

        WindowCompat.setDecorFitsSystemWindows(window, false);

        View decorView = window.getDecorView();
        decorView.setBackgroundColor(Color.BLACK);
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION);

        // Apply bottom inset padding so Qt content (TabBar) is above navigation bar on older Android.
        View contentView = window.findViewById(android.R.id.content);
        contentView.setBackgroundColor(Color.BLACK);

        ViewCompat.setOnApplyWindowInsetsListener(contentView, (v, insets) -> {
            int bottomInset = insets.getInsets(WindowInsetsCompat.Type.navigationBars()).bottom;
            v.setPadding(0, 0, 0, bottomInset);
            return insets;
        });
        ViewCompat.requestApplyInsets(contentView);
    }
}
