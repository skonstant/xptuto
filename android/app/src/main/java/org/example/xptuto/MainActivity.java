package org.example.xptuto;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.widget.ContentLoadingProgressBar;

import com.bumptech.glide.Glide;

import java.text.DateFormat;
import java.util.Locale;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = "XP";
    private static final String LOADED = "arg_loaded";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private ContentLoadingProgressBar progressBar;
    private TextView notFound;
    private View details;
    private EditText editText;
    private TextView userName;
    private TextView creationDate;
    private ImageView avatar;

    private Xptuto x;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        x = Xptuto.makeInstance(new JavaHttpClient(getApplicationContext()), new AndroidThreads());

        progressBar = findViewById(R.id.progress);
        notFound = findViewById(R.id.notFound);
        details = findViewById(R.id.details);
        editText = findViewById(R.id.user_name_input);
        userName = findViewById(R.id.user_name);
        creationDate = findViewById(R.id.creation_date);
        avatar = findViewById(R.id.avatar);

        editText.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_SEARCH) {
                InputMethodManager imm = (InputMethodManager) getSystemService(Activity.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(v.getWindowToken(), 0);

                details.setVisibility(View.INVISIBLE);
                notFound.setVisibility(View.INVISIBLE);
                progressBar.show();

                x.getUser(v.getText().toString(), new GetUserCb() {
                    @Override
                    public void onSuccess(User user) {
                        Log.d(TAG, "Got a user with name: " + user.login);
                        progressBar.hide();
                        details.setVisibility(View.VISIBLE);
                        userName.setText(user.login);
                        creationDate.setText(DateFormat.getDateInstance(
                                DateFormat.MEDIUM, Locale.getDefault()).format(user.createdAt));
                        avatar.setImageDrawable(null);
                        Glide.with(MainActivity.this)
                                .load(user.avatarUrl).into(avatar);
                    }

                    @Override
                    public void onError(String error) {
                        Log.e(TAG, "Error: " + error);
                        progressBar.hide();
                        notFound.setVisibility(View.VISIBLE);
                    }
                });

                return true;
            }
            return false;
        });

        if (savedInstanceState != null && savedInstanceState.getBoolean(LOADED)) {
            editText.post(() -> editText.onEditorAction(EditorInfo.IME_ACTION_SEARCH));
        }
    }

    public void viewRepositories(View view) {
        if (!TextUtils.isEmpty(userName.getText())) {
            Intent intent = new Intent(this, ListActivity.class);
            intent.putExtra(ListActivity.USER_NAME, userName.getText().toString());
            startActivity(intent);
        }
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        if (!TextUtils.isEmpty(userName.getText())) {
            outState.putBoolean(LOADED, true);
        }
        super.onSaveInstanceState(outState);
    }
}
