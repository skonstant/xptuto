// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

package org.example.xptuto;

import java.util.concurrent.atomic.AtomicBoolean;

public abstract class HttpClient {
    public abstract void get(String url, HttpCallback callback);

    public abstract HttpResponse getSync(String url);

    private static final class CppProxy extends HttpClient
    {
        private final long nativeRef;
        private final AtomicBoolean destroyed = new AtomicBoolean(false);

        private CppProxy(long nativeRef)
        {
            if (nativeRef == 0) throw new RuntimeException("nativeRef is zero");
            this.nativeRef = nativeRef;
        }

        private native void nativeDestroy(long nativeRef);
        public void _djinni_private_destroy()
        {
            boolean destroyed = this.destroyed.getAndSet(true);
            if (!destroyed) nativeDestroy(this.nativeRef);
        }
        protected void finalize() throws java.lang.Throwable
        {
            _djinni_private_destroy();
            super.finalize();
        }

        @Override
        public void get(String url, HttpCallback callback)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            native_get(this.nativeRef, url, callback);
        }
        private native void native_get(long _nativeRef, String url, HttpCallback callback);

        @Override
        public HttpResponse getSync(String url)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getSync(this.nativeRef, url);
        }
        private native HttpResponse native_getSync(long _nativeRef, String url);
    }
}
