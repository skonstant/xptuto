package org.example.xptuto;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Xptuto x = Xptuto.makeInstance();
        //for (User u : x.getUsers()) {
        //    Log.d("XP", "Got a user with name: " + u.name);
        //}
    }
}
