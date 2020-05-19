package org.example.xptuto;

import android.os.Handler;
import android.os.Looper;

public class AndroidThreads extends PlatformThreads {
    @Override
    public void createThread(String name, final ThreadFunc func) {
        new Thread(func::run, name).start();
    }

    @Override
    public void runOnMainThread(final ThreadFunc func) {
        new Handler(Looper.getMainLooper()).post(func::run);
    }

    @Override
    public boolean isMainThread() {
        return Looper.getMainLooper() == Looper.myLooper();
    }
}
