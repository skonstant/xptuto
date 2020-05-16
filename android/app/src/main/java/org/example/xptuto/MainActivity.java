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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Xptuto x = Xptuto.makeInstance(new JavaHttpClient(getApplicationContext()));
        x.getUsers(new GetUsersCb() {
            @Override
            public void onSuccess(ArrayList<User> users) {
                for (User u : users) {
                    Log.d(TAG, "Got a user with name: " + u.login);
                }
            }

            @Override
            public void onError(String error) {
                Log.d(TAG, "Error: " + error);
            }
        });

    }
}
