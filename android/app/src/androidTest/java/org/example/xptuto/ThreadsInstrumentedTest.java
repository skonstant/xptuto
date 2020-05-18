package org.example.xptuto;

import android.os.Looper;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class ThreadsInstrumentedTest {
    @Test
    public void backgroundThread() throws InterruptedException {
        new AndroidThreads().createThread("background", new ThreadFunc() {
            @Override
            public void run() {
                assertNotEquals(Looper.myLooper(), Looper.getMainLooper());
                synchronized (ThreadsInstrumentedTest.this) {
                    ThreadsInstrumentedTest.this.notify();
                }
            }
        });

        synchronized (this) {
            this.wait();
        }
    }

    @Test
    public void mainThread() {
        new AndroidThreads().runOnMainThread(new ThreadFunc() {
            @Override
            public void run() {
                assertEquals(Looper.getMainLooper(), Looper.getMainLooper());
            }
        });
    }
}
