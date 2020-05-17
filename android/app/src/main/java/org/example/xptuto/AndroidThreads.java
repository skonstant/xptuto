package org.example.xptuto;

import android.os.Handler;
import android.os.Looper;

public class AndroidThreads extends PlatformThreads {
    @Override
    public void createThread(String name, final ThreadFunc func) {
        Thread thread = new Thread(name) {
            @Override
            public void run() {
                super.run();
                func.run();
            }
        };
        thread.setDaemon(true);
        thread.start();
    }

    @Override
    public void runOnMainThread(final ThreadFunc func) {
        new Handler(Looper.getMainLooper()).post(new Runnable() {
            @Override
            public void run() {
                func.run();
            }
        });
    }

    @Override
    public boolean isMainThread() {
        return Looper.getMainLooper() == Looper.myLooper();
    }
}
