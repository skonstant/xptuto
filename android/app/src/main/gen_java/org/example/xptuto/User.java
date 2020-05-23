// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

package org.example.xptuto;

import java.util.Date;

public final class User {


    /*package*/ final String login;

    /*package*/ final int id;

    /*package*/ final String avatarUrl;

    /*package*/ final Date createdAt;

    public User(
            String login,
            int id,
            String avatarUrl,
            Date createdAt) {
        this.login = login;
        this.id = id;
        this.avatarUrl = avatarUrl;
        this.createdAt = createdAt;
    }

    public String getLogin() {
        return login;
    }

    public int getId() {
        return id;
    }

    public String getAvatarUrl() {
        return avatarUrl;
    }

    public Date getCreatedAt() {
        return createdAt;
    }

    @Override
    public String toString() {
        return "User{" +
                "login=" + login +
                "," + "id=" + id +
                "," + "avatarUrl=" + avatarUrl +
                "," + "createdAt=" + createdAt +
        "}";
    }

}