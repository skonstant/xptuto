package org.example.xptuto;

import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "XP";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private Xptuto x;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        x = Xptuto.makeInstance(new JavaHttpClient(getApplicationContext()), new AndroidThreads());
        x.getUser("aosp", new GetUserCb() {
            @Override
            public void onSuccess(User user) {
                Log.d(TAG, "Got a user with name: " + user.login);
                x.getReposForUser(user, new GetReposCb() {
                    @Override
                    public void onSuccess(ArrayList<Repo> repos, User u) {
                        for (Repo repo : repos) {
                            Log.d(TAG, "Got a repo with name: " + repo.name);
                        }
                    }

                    @Override
                    public void onError(String error) {
                        Log.e(TAG, "Error: " + error);
                    }
                });
            }

            @Override
            public void onError(String error) {
                Log.e(TAG, "Error: " + error);
            }
        });

    }
}
