package org.example.xptuto;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
import okhttp3.ResponseBody;

public class JavaHttpClient extends HttpClient {

    private final OkHttpClient client = new OkHttpClient();

    @Override
    public void get(String url, final HttpCallback callback) {
        Request request = new Request.Builder()
                .url(url)
                .build();
        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(@NotNull Call call, @NotNull IOException e) {
                callback.onFailure(e.getMessage());
            }

            @Override
            public void onResponse(@NotNull Call call, @NotNull Response response) throws IOException {
                String body = null;
                ResponseBody b = response.body();
                if (b != null) {
                    try {
                        body = b.string();
                    } catch (IOException unused) {
                    }
                }
                callback.onResponse(new HttpResponse(body, response.code()));
            }
        });
    }
}
