package org.example.xptuto;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.RecyclerView;

import org.jetbrains.annotations.NotNull;

import java.text.DateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

public class ListActivity extends AppCompatActivity {

    public static final String USER_NAME = "arg_user_name";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list);

        RecyclerView recyclerView = findViewById(R.id.recycler_view);
        recyclerView.setAdapter(new MyAdapter(getIntent().getStringExtra(USER_NAME)));
    }

    public static class MyViewHolder extends RecyclerView.ViewHolder {
        public final TextView name;
        public final TextView description;
        final TextView creationDate;

        MyViewHolder(View v) {
            super(v);
            name = v.findViewById(R.id.name);
            description = v.findViewById(R.id.descr);
            creationDate = v.findViewById(R.id.creation_date);
        }
    }

    public class MyAdapter extends RecyclerView.Adapter<MyViewHolder> {
        private List<Repo> repos;

        MyAdapter(String userName) {
            Xptuto x = Xptuto.getInstance();
            if (x == null) {
                x = Xptuto.makeInstance(new JavaHttpClient(getApplicationContext()), new AndroidThreads(), getCacheDir().getAbsolutePath());
            }
            x.getReposForUserName(userName, new GetReposCb() {
                @Override
                public void onSuccess(ArrayList<Repo> r, User u) {
                    if (!isFinishing() && !isDestroyed()) {
                        repos = r;
                        notifyDataSetChanged();
                    }
                }

                @Override
                public void onError(String error) {
                    if (!isFinishing() && !isDestroyed()) {
                        finish();
                    }
                }
            });
        }

        @NotNull
        @Override
        public MyViewHolder onCreateViewHolder(ViewGroup parent,
                                               int viewType) {
            return new MyViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item, parent, false));
        }

        @Override
        public void onBindViewHolder(@NotNull MyViewHolder holder, int position) {
            Repo repo = repos.get(position);
            String str = repo.name + " (" + repo.fullName + ")";
            if (repo.priv) {
                str += " - private";
            }
            holder.name.setText(str);
            if (!TextUtils.isEmpty(repo.descr)) {
                holder.description.setText(repo.descr);
            } else {
                holder.description.setText("No description");
            }

            holder.creationDate.setText(getString(R.string.created_on, DateFormat.getDateInstance(
                    DateFormat.LONG, Locale.getDefault()).format(repo.createdAt)));
        }

        @Override
        public int getItemCount() {
            return repos == null ? 0 : repos.size();
        }
    }

}
