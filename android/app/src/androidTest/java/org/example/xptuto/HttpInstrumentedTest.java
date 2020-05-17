package org.example.xptuto;

import androidx.test.core.app.ApplicationProvider;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.net.HttpURLConnection;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class HttpInstrumentedTest {
    @Test
    public void getUser() throws InterruptedException {
        new JavaHttpClient(ApplicationProvider.getApplicationContext()).get("https://api.github.com/users/aosp", new HttpCallback() {
            @Override
            public void onFailure(String reason) {
                fail();
                synchronized (HttpInstrumentedTest.this) {
                    HttpInstrumentedTest.this.notify();
                }
            }

            @Override
            public void onResponse(String body, int code) {
                assertEquals(code, HttpURLConnection.HTTP_OK);
                synchronized (HttpInstrumentedTest.this) {
                    HttpInstrumentedTest.this.notify();
                }
            }
        });

        synchronized (this) {
            this.wait();
        }
    }

    @Test
    public void get404() throws InterruptedException {
        new JavaHttpClient(ApplicationProvider.getApplicationContext()).get("https://api.github.com/users/aospppp", new HttpCallback() {
            @Override
            public void onFailure(String reason) {
                fail();
                synchronized (HttpInstrumentedTest.this) {
                    HttpInstrumentedTest.this.notify();
                }
            }

            @Override
            public void onResponse(String body, int code) {
                assertEquals(code, HttpURLConnection.HTTP_NOT_FOUND);
                synchronized (HttpInstrumentedTest.this) {
                    HttpInstrumentedTest.this.notify();
                }
            }
        });

        synchronized (this) {
            this.wait();
        }
    }

    @Test
    public void getError() throws InterruptedException {
        new JavaHttpClient(ApplicationProvider.getApplicationContext()).get("https://api.githubbbb.com/users/aospppp", new HttpCallback() {
            @Override
            public void onFailure(String reason) {
                synchronized (HttpInstrumentedTest.this) {
                    HttpInstrumentedTest.this.notify();
                }
            }

            @Override
            public void onResponse(String body, int code) {
                fail();
                synchronized (HttpInstrumentedTest.this) {
                    HttpInstrumentedTest.this.notify();
                }
            }
        });

        synchronized (this) {
            this.wait();
        }
    }

    @Test
    public void getUserSync() {
        HttpResponse response = new JavaHttpClient(ApplicationProvider.getApplicationContext())
                .getSync("https://api.github.com/users/aosp");
        assertEquals(response.code.intValue(), 200);
    }

    @Test
    public void get404Sync() {
        HttpResponse response = new JavaHttpClient(ApplicationProvider.getApplicationContext())
                .getSync("https://api.github.com/users/aosppppp");
        assertEquals(response.code.intValue(), 404);
    }

    @Test
    public void getErrorSync() {
        try {
            new JavaHttpClient(ApplicationProvider.getApplicationContext())
                    .getSync("https://api.githubbbbb.com/users/aosppppp");
            fail();
        } catch (Exception e) {

        }
    }
}
